using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PasswordManager
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			string saveDir = null;
			for (uint i = 0; i < args.Length; i++)
			{
				if (args[i] == "-f")
					saveDir = args[i + 1];
			}
			if (saveDir == null)
			{
				MessageBox.Show("No save directory was set! please set one using -f \"directory\"");
			}


			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			string key = PasswordInput.GetInput();
			if (key.Length != 0)
				Application.Run(new PasswordManager(key, saveDir));
		}
	}
}
