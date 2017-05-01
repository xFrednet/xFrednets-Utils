using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Linq;

namespace PasswordManager
{

	public partial class PasswordManager : Form
	{
		private const string SAVE_FILE = "DataSaveFile.xml";
		private const string BACKUP_FILE = "DataSaveBackup.xml";
		private const int DATA_PER_ROW = 3;

		private const int MODE_COUNT = 3;
		private const int MODE_RETRIEVE = 0;
		private const int MODE_CHANGE  = 1;
		private const int MODE_REMOVE  = 2;
		private readonly string[] MODE_NAMES = {"Retrieve Mode", "Change Mode", "Remove Mode"};
		private readonly string[] MODE_INFO = {"Click on a label to retrieve the Data.", "Click on a label to change the Data.", "Clock on a label to remove the Data"};
		private readonly Color[] MODE_COLOR = { Color.LightGreen, Color.Blue, Color.Red };

		int Mode;
		Crypter crypter;
		List<DataTab> tabList;

		public PasswordManager(string key)
		{
			if (key.Length == 0)
				key = "No Key";
			
			InitializeComponent();
			updateMode(MODE_RETRIEVE);
			crypter = new Crypter(key);
			loadDataFromFile(SAVE_FILE);
			updateGUI();
		}

		private void Form1_Load(object sender, EventArgs e)
		{

		}
		
		//
		// Update
		//
		public void updateGUI()
		{
			//
			// getting the data Tab
			//
			int selectedIndex = dataTabControl.SelectedIndex;
			dataTabControl.Controls.Clear();
			int index = 0;
			foreach (DataTab tab in tabList) {

				//
				// TabPage
				//
				int dataCount = tab.DataList.Count;
				if (tab.TabPage == null || tab.Layout == null)
				{
					tab.TabPage = new TabPage();
					tab.TabPage.SuspendLayout();
					tab.Layout = new TableLayoutPanel();

					//
					// tab.Layout
					//
					tab.Layout.Dock			= DockStyle.Fill;
					tab.Layout.Location		= new Point(4, 4);
					tab.Layout.AutoScroll	= true;
					tab.Layout.Padding		= new Padding(0, 0, 4, 0);

					//
					// tab.TabPage
					//
					tab.TabPage.Controls.Add(tab.Layout);
					tab.TabPage.Dock = DockStyle.Fill;
					tab.TabPage.Margin = new Padding(4);
					tab.TabPage.Name = tab.Name + "TabPage";
					tab.TabPage.Text = tab.Name;
					tab.TabPage.UseVisualStyleBackColor = true;
					tab.TabPage.TabIndex = index;
					tab.TabPage.ResumeLayout(false);

				}
				dataTabControl.Controls.Add(tab.TabPage);

				updateTabGUI(tab);
				index++;
			}
			dataTabControl.SelectedIndex = selectedIndex;

			//
			// index input
			//
			addTabIndexInput.Maximum = tabList.Count;
			addTabIndexInput.Value = tabList.Count;

			if (dataTabControl.SelectedTab != null)
			{
				string tabName = dataTabControl.SelectedTab.Text;
				foreach (DataTab tab in tabList)
				{
					if (tab.Name == tabName)
					{
						addDataIndexInput.Maximum = tab.DataList.Count;
						addDataIndexInput.Value = tab.DataList.Count;
						break;
					}
				}
			}

		}
		public void updateTabGUI(DataTab tab)
		{
			if (tab == null || tab.Layout == null)
				return;

			tab.Layout.SuspendLayout();
			tab.Layout.Controls.Clear();
			int dataCount = tab.DataList.Count;
			int i = 0;
			//
			// TableLayout
			//
			tab.Layout.ColumnStyles.Clear();
			tab.Layout.ColumnCount = DATA_PER_ROW;
			for (i = 0; i < tab.Layout.ColumnCount; i++)
				tab.Layout.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F / DATA_PER_ROW));

			tab.Layout.RowStyles.Clear();
			tab.Layout.RowCount = (dataCount / DATA_PER_ROW) + ((dataCount % DATA_PER_ROW != 0) ? 2 : 1);
			for (i = 0; i < tab.Layout.RowCount; i++)
				tab.Layout.RowStyles.Add(new RowStyle(SizeType.Absolute, 50F));

			//
			// Data Labels
			//
			int x;
			int y;
			i = 0;
			foreach(DataStruct data in tab.DataList)
			{
				y = i / DATA_PER_ROW;
				x = i % DATA_PER_ROW;

				if (data.Button == null)
				{
					data.Button = new Button();
					data.Button.Dock = DockStyle.Fill;
					data.Button.Name = data.Name + "NameLabel";
					data.Button.Text = data.Name;
					data.Button.Click += new EventHandler(data.nameLabel_Click);
				}

				tab.Layout.Controls.Add(data.Button, x, y);

				i++;
			}
			tab.Layout.ResumeLayout();
		}
		public void updateMode(int mode)
		{
			Mode = mode;
			Button button = modeSwitch1;
			for (int i = 0; i < MODE_COUNT; i++)
			{
				if (i == Mode)
				{
					modeInfoLabel.Text			= MODE_INFO[Mode];
					modeColorPanel.BackColor	= MODE_COLOR[Mode];
					continue;
				}

				button.Text = MODE_NAMES[i];
				button = modeSwitch2;
			}
		}

		//
		// Data loaders
		//
		// <Tabs>
		//   <Tab name="Main">
		//     <Data name="name">data<\Data>
		//   </Tab>
		// <Tabs>
		private void addData(string tabName, string name, string data, int index)
		{

			bool added = false;
			foreach(DataTab tab in tabList)
			{
				if (tab.Name == tabName)
				{
					DataStruct newData = new DataStruct(tab);
					newData.Name = addDataNameInput.Text;
					newData.Value = addDataDataInput.Text;

					if (index > tab.DataList.Count)
						index = tab.DataList.Count;

					tab.DataList.Insert(index, newData);
					updateTabGUI(tab);
					added = true;
					break;
				}
			}
			if (!added)
				Console.WriteLine("The tab name was not fount: " + tabName);
			else
				saveData(SAVE_FILE);

			if (dataTabControl.SelectedTab != null)
			{
				foreach (DataTab tab in tabList)
				{
					if (tab.Name == tabName)
					{
						addDataIndexInput.Maximum = tab.DataList.Count;
						addDataIndexInput.Value = tab.DataList.Count;
						break;
					}
				}
			}
		}
		private void removeData(DataStruct data)
		{
			data.ParentTab.DataList.Remove(data);
			saveData(SAVE_FILE);
			updateGUI();
		}
		private void addTab(string name, int index)
		{
			foreach (DataTab tab in tabList)
				if (tab.Name == name)
					return;
			

			if (index < 1)
				index = 1;
			if (index > tabList.Count)
				index = tabList.Count;
			DataTab newTab = new DataTab(name, this);
			tabList.Insert(index, newTab);

			saveData(SAVE_FILE);

			updateGUI();
		}
		private void removeTab(string name)
		{
			DialogResult dialogResult = MessageBox.Show("Do you want to delete the current tab named: \"" + name + "\"?", "Are you sure??", MessageBoxButtons.YesNo);

			int i = 0;
			if (dialogResult == DialogResult.Yes)
			{
				foreach (DataTab tab in tabList)
				{
					if (tab.Name == name)
						break;
					i++;
				}

				tabList.RemoveAt(i);
				saveData();
				updateGUI();
			}
		}

		private void loadDataFromFile(string file)
		{
			if (tabList == null)
				tabList = new List<DataTab>();

			string xmlString;
			try
			{
				xmlString = File.ReadAllText(file, Encoding.UTF8);
			} catch (FileNotFoundException e)
			{
				xmlString = "";
				MessageBox.Show("The save file couldn't be loaded and will be created.");
			}
			if (xmlString.Length != 0)
			{
				XmlDocument xml = new XmlDocument();
				xml.LoadXml(xmlString);
				//<Tabs>
				foreach (XmlNode xmlChild in xml.ChildNodes) {
					if (xmlChild.Name != "Tabs")
						continue;

					foreach (XmlElement xmlTab in xmlChild.ChildNodes)
					{
						if (xmlTab.Name != "Tab")
							continue;
						//<Tab name="$Name$">
						DataTab tab = new DataTab(xmlTab.GetAttribute("name"), this);

						foreach (XmlElement xmlData in xmlTab.ChildNodes)
						{
							if (xmlData.Name != "Data")
								continue;

							//<Data>
							DataStruct data = new DataStruct(tab);

							foreach (XmlElement xmlDataChild in xmlData.ChildNodes)
							{
								switch (xmlDataChild.Name)
								{
									case "Name":
										data.Name = crypter.decrypt(hexToString(xmlDataChild.InnerText));
										break;
									case "Value":
										data.Value = crypter.decrypt(hexToString(xmlDataChild.InnerText));
										break;
									default:
										Console.WriteLine("Unknown XmlChild from <Data>:" + xmlDataChild.Name + " " + xmlDataChild.InnerText + " " + (xmlDataChild.Name == "Name") + " " + (xmlDataChild.Name == "Value"));
										break;
								}
							}

							//</Data>
							tab.DataList.Add(data);
						}
						//</Tab>
						tabList.Add(tab);
					}
					//</Tabs>
				}
			} else
			{
				tabList.Add(new DataTab("Main", this));
				saveData();
			}
		}
		public void saveData()
		{
			saveData(SAVE_FILE);
		}
		public void saveData(string file)
		{
			XmlDocument xml = new XmlDocument();
			xml.AppendChild(xml.CreateXmlDeclaration("1.0", "UTF-8", null));

			XmlElement tabs = xml.CreateElement("Tabs");
			//<Tabs>
			foreach(DataTab tab in tabList)
			{
				//<Tab name="$Name$">
				XmlElement xmlTab = xml.CreateElement("Tab");
				xmlTab.SetAttribute("name", tab.Name);

				foreach(DataStruct data in tab.DataList)
				{
					//<Data>
					XmlElement xmlData = xml.CreateElement("Data");

					//    <Name>
					XmlElement xmlDataName = xml.CreateElement("Name");
					xmlDataName.InnerText = stringToHex(crypter.encrypt(data.Name));
					xmlData.AppendChild(xmlDataName);
					//    </Name>

					//    <Value>
					XmlElement xmlDataData = xml.CreateElement("Value");
					xmlDataData.InnerText = stringToHex(crypter.encrypt(data.Value));
					xmlData.AppendChild(xmlDataData);
					//    </Value>

					xmlTab.AppendChild(xmlData);
					//</Data>
				}
				tabs.AppendChild(xmlTab);
				//</Tab>
			}
			//</Tabs>

			xml.AppendChild(tabs);

			using (TextWriter sw = new StreamWriter(file, false, Encoding.UTF8)) //Set encoding
			{
				xml.Save(sw);
			}
			
		}
		private string stringToHex(string input)
		{
			char[] chars = input.ToCharArray();
			string str = "";
			foreach (char c in chars)
			{
				str += String.Format("{0:X4}", Convert.ToInt32(c));
			}
			return str;
		}
		private string hexToString(string hexStr)
		{
			int count = hexStr.Length / 4;
			char[] str = new char[count];
			string s;

			for (int i = 0; i < count; i++)
			{
				s = hexStr.Substring(i * 4, 4);
				str[i] = (char)Int16.Parse(s, System.Globalization.NumberStyles.HexNumber);
			}

			return new string(str);
		}
		private void saveBackup()
		{
			string backupFileName = "Backup"  + DateTime.Now.ToShortDateString()  + "." + DateTime.Now.ToLongTimeString().Replace(':', '.') + ".xml";
			saveData(backupFileName);
		}

		//
		// Click actions
		//
		private void addTabButton_Click(object sender, EventArgs e)
		{
			addTab(addTabNameInput.Text, (int)addTabIndexInput.Value);
		}
		private void addDataButton_Click(object sender, EventArgs e)
		{
			addData(dataTabControl.SelectedTab.Text, addDataNameInput.Text, addDataDataInput.Text, (int)addDataIndexInput.Value);
		}
		private void modeSwitchButton_Click(object sender, EventArgs e)
		{
			Button button = (Button)sender;
			for (int i = 0; i < MODE_COUNT; i++)
			{
				if (MODE_NAMES[i] == button.Text)
				{
					updateMode(i);
					return;
				}
			}
		}
		private void dataTabControl_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (dataTabControl.SelectedTab == null)
				return;

			string tabName = dataTabControl.SelectedTab.Text;
			foreach(DataTab tab in tabList)
			{
				if (tab.Name == tabName)
				{
					addDataIndexInput.Maximum = tab.DataList.Count;
					addDataIndexInput.Value = tab.DataList.Count;
					return;
				}
			}
		}
		private void changeKeyButton_Click(object sender, EventArgs e)
		{
			string key = PasswordInput.GetInput("Please enter your new Key:", false);
			if (key.Length == 0)
				return;

			saveData(BACKUP_FILE);
			crypter = new Crypter(key);
			saveData();
		}
		private void deleteTabButton_Click(object sender, EventArgs e)
		{
			if (dataTabControl.SelectedTab != null)
				removeTab(dataTabControl.SelectedTab.Text);
		}
		private void makeBackupButton_Click(object sender, EventArgs e)
		{
			saveBackup();
		}
		internal void clickOnData(DataStruct data)
		{
			switch (Mode)
			{
				case MODE_RETRIEVE:
					if (ModifierKeys == Keys.Shift)
					{
						Clipboard.SetText(data.Value);
						Dispose();
						return;
					} else if (ModifierKeys == Keys.Control)
					{
						Clipboard.SetText(data.Value);
					} else
					{
						infoLabelName.Text = data.Name;
						infoLabelData.Text = data.Value;
					}

					break;
				case MODE_CHANGE:
					Form f = new ChangeDataWindow(this, data);
					f.ShowDialog(this);
					break;
				case MODE_REMOVE:
					removeData(data);
					break;
			}
		}

	}

	public class DataStruct
	{
		public string Name;
		public string Value;
		public Button Button;
		public DataTab ParentTab;

		public DataStruct(DataTab parentTab)
		{
			Button = null;
			ParentTab = parentTab;
		}
		public void nameLabel_Click(object sender, EventArgs e)
		{
			ParentTab.Manager.clickOnData(this);
		}

		public void updateLabel()
		{
			Button.Text = Name;
		}
	}
	public class DataTab
	{
		public string           Name;
		public List<DataStruct> DataList;

		public TabPage          TabPage;
		public TableLayoutPanel Layout;

		public PasswordManager Manager;

		public DataTab(string name, PasswordManager manager)
		{
			Name = name;
			DataList = new List<DataStruct>();
			TabPage = null;
			Layout = null;
			Manager = manager;
		}
	}
}
