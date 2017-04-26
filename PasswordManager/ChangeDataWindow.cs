using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PasswordManager
{
	public partial class ChangeDataWindow : Form
	{
		DataStruct data;
		PasswordManager manager;

		public ChangeDataWindow(PasswordManager manager, DataStruct data)
		{
			this.data = data;
			this.manager = manager;
			InitializeComponent();
			Console.WriteLine(data.Name + " " + data.Value);
			nameInput.Text = data.Name;
			dataInput.Text = data.Value;
		}

		private void cancelButton_Click(object sender, EventArgs e)
		{
			Dispose();
		}

		private void saveButton_Click(object sender, EventArgs e)
		{
			data.Name = nameInput.Text;
			data.Value = dataInput.Text;
			data.updateLabel();
			manager.saveData();
			Dispose();
		}
	}
}
