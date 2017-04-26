using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PasswordManager
{
	public partial class PasswordInput : Form
	{

		public static string GetInput()
		{
			return GetInput("Please enter your Key:", true);
		}
		public static string GetInput(string text, bool hiddenText)
		{
			PasswordInput input = new PasswordInput(text, hiddenText);
			//Application.Run(input);
			input.ShowDialog();
			return input.text;
		}
		string text;

		private PasswordInput(string text, bool hiddenText)
		{
			this.text = "";
			InitializeComponent();
			AcceptButton = doneButton;
			textInput.UseSystemPasswordChar = hiddenText;
			mainText.Text = text;
		}

		private void cancelButton_Click(object sender, EventArgs e)
		{
			text = "";
			Dispose();
		}
		private void doneButton_Click(object sender, EventArgs e)
		{
			text = textInput.Text;
			Dispose();
		}
	}
}
