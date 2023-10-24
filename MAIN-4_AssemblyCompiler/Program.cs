using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;
using System.Reflection;

namespace MAIN_4_AssemblyCompiler
{
    internal class Program
    {
        static Dictionary<string, string> Definitions = new Dictionary<string, string>();
        static List<Tuple<string, int, List<string>>> Macros = new List<Tuple<string, int, List<string>>>();

        static void Main(string[] args)
        {
            string code = File.ReadAllText(args[0]);

            // parse
            List<string> tokens = Parse(code);

            // expanding uses
            while (tokens.Contains("use"))
            {
                int start = tokens.IndexOf("use");
                string file = Directory.GetCurrentDirectory() + "\\libs\\" + tokens[start + 1] + ".s";

                tokens.RemoveAt(start);
                tokens.RemoveAt(start);

                tokens.InsertRange(start, Parse(File.ReadAllText(file)));
            }

            // debug info
            Console.WriteLine("Parsed:");
            foreach (string token in tokens)
                Console.Write(token + "; ");

            // code gen
            byte[] result = new byte[512];
            for (int i = 0; i < tokens.Count; i++) 
            {
                if (Definitions.ContainsKey(tokens[i]))
                    tokens[i] = Definitions[tokens[i--]];
                else if (Macros.LastOrDefault((a) => a.Item1 == tokens[i]) != null)
                {
                    Tuple<string, int, List<string>> macro = Macros.LastOrDefault((a) => a.Item1 == tokens[i]);
                    List<string> conts = macro.Item3.ToArray().ToList();

                    tokens.RemoveAt(i);
                    int index = 0;
                    for (int x = 0; x < macro.Item2; tokens.RemoveAt(i), x++)
                        while ((index = conts.FindIndex(s => s == "@" + x)) != -1)
                            conts[index] = tokens[i];

                    tokens.InsertRange(i, conts);
                    i--;

                }
                else if (tokens[i] == "mac")
                {
                    string name = tokens[i + 1];
                    int count = int.Parse(tokens[i + 2]);
                    List<string> conts = new List<string>();

                    tokens.RemoveAt(i);
                    tokens.RemoveAt(i);
                    tokens.RemoveAt(i);
                    for (; tokens[i] != "eom"; tokens.RemoveAt(i))
                        conts.Add(tokens[i]);
                    tokens.RemoveAt(i);
                    i--;

                    Macros.Add(new Tuple<string, int, List<string>>(name, count, conts));
                }
                else if (tokens[i] == "def")
                {
                    Definitions[tokens[i + 1]] = tokens[i + 2];
                    tokens.RemoveAt(i);
                    tokens.RemoveAt(i);
                    tokens.RemoveAt(i);
                    i--;
                }
                else if (i < tokens.Count - 1 && tokens[i + 1] == ":")
                {
                    result[i] = 14; // flg
                    result[i + 1] = (byte)Convert.ToInt32(tokens[i]);
                    i++;
                }
                else if (int.TryParse(tokens[i], out int value) && value < 16)
                    result[i] = (byte)value;
                else
                    Error("Unknown token: " + tokens[i] + "!");
            }

            // definitions
            Console.WriteLine("\n\nDefinitions:");
            foreach (KeyValuePair<string, string> def in Definitions)
                Console.WriteLine(def.Key + ": " + def.Value);

            // macros
            Console.WriteLine("\n\nMacros:");
            foreach (Tuple<string, int, List<string>> mac in Macros)
            {
                Console.Write(mac.Item1 + " " + mac.Item2 + ": ");
                foreach (string s in mac.Item3)
                    Console.Write(s + " ");
                Console.WriteLine();
            }

            // generated
            Console.WriteLine("\nGenerated:");
            foreach (byte res in result)
                Console.Write(Convert.ToString(res, 2).PadLeft(4, '0') + " ");

            Console.WriteLine("\n\n" + tokens.Count + " / 512");
            File.WriteAllBytes(Directory.GetCurrentDirectory() + "\\ROM.b", result);
            Console.Read();
        }

        static void Error(string message)
        {
            Console.BackgroundColor = ConsoleColor.Red;
            Console.ForegroundColor = ConsoleColor.Black;
            Console.WriteLine(message);
            Console.Read();
            Environment.Exit(0);
        }
        
        public static List<string> Parse(string code)
        {
            // preprocessor
            code = Regex.Replace(code, ";.*\\n", " ");
            code = Regex.Replace(code, ":", " : ");
            code = Regex.Replace(code, "\\s", " ");
            code = Regex.Replace(code, " +", " ");
            code = code.Trim().ToLower();

            return code.Split(' ').ToList();
        }
    }
}
