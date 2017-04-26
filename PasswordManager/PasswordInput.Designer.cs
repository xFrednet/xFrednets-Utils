namespace PasswordManager
{
	partial class PasswordInput
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.mainLayout = new System.Windows.Forms.TableLayoutPanel();
			this.mainText = new System.Windows.Forms.Label();
			this.textInput = new System.Windows.Forms.TextBox();
			this.cancelButton = new System.Windows.Forms.Button();
			this.doneButton = new System.Windows.Forms.Button();
			this.mainLayout.SuspendLayout();
			this.SuspendLayout();
			// 
			// mainLayout
			// 
			this.mainLayout.ColumnCount = 2;
			this.mainLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.mainLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.mainLayout.Controls.Add(this.mainText, 0, 0);
			this.mainLayout.Controls.Add(this.textInput, 0, 1);
			this.mainLayout.Controls.Add(this.cancelButton, 0, 2);
			this.mainLayout.Controls.Add(this.doneButton, 1, 2);
			this.mainLayout.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainLayout.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.mainLayout.Location = new System.Drawing.Point(0, 0);
			this.mainLayout.Name = "mainLayout";
			this.mainLayout.RowCount = 3;
			this.mainLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
			this.mainLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
			this.mainLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
			this.mainLayout.Size = new System.Drawing.Size(284, 92);
			this.mainLayout.TabIndex = 0;
			// 
			// mainText
			// 
			this.mainText.AutoSize = true;
			this.mainLayout.SetColumnSpan(this.mainText, 2);
			this.mainText.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainText.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.mainText.Location = new System.Drawing.Point(3, 0);
			this.mainText.Name = "mainText";
			this.mainText.Size = new System.Drawing.Size(278, 30);
			this.mainText.TabIndex = 0;
			this.mainText.Text = "Please enter your Key:";
			this.mainText.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// textInput
			// 
			this.textInput.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.mainLayout.SetColumnSpan(this.textInput, 2);
			this.textInput.Location = new System.Drawing.Point(3, 35);
			this.textInput.Name = "textInput";
			this.textInput.Size = new System.Drawing.Size(278, 20);
			this.textInput.TabIndex = 0;
			this.textInput.UseSystemPasswordChar = true;
			// 
			// cancelButton
			// 
			this.cancelButton.Dock = System.Windows.Forms.DockStyle.Fill;
			this.cancelButton.Location = new System.Drawing.Point(3, 63);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(136, 26);
			this.cancelButton.TabIndex = 2;
			this.cancelButton.Text = "Cancel";
			this.cancelButton.UseVisualStyleBackColor = true;
			this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
			// 
			// doneButton
			// 
			this.doneButton.Dock = System.Windows.Forms.DockStyle.Fill;
			this.doneButton.Location = new System.Drawing.Point(145, 63);
			this.doneButton.Name = "doneButton";
			this.doneButton.Size = new System.Drawing.Size(136, 26);
			this.doneButton.TabIndex = 1;
			this.doneButton.Text = "Done";
			this.doneButton.UseVisualStyleBackColor = true;
			this.doneButton.Click += new System.EventHandler(this.doneButton_Click);
			// 
			// PasswordInput
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 92);
			this.Controls.Add(this.mainLayout);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "PasswordInput";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Password Input";
			this.mainLayout.ResumeLayout(false);
			this.mainLayout.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel mainLayout;
		private System.Windows.Forms.Label mainText;
		private System.Windows.Forms.TextBox textInput;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Button doneButton;
	}
}