namespace PasswordManager
{
	partial class ChangeDataWindow
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
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.nameLabel = new System.Windows.Forms.Label();
			this.nameInput = new System.Windows.Forms.TextBox();
			this.dataLabel = new System.Windows.Forms.Label();
			this.dataInput = new System.Windows.Forms.TextBox();
			this.cancelButton = new System.Windows.Forms.Button();
			this.saveButton = new System.Windows.Forms.Button();
			this.tableLayoutPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.ColumnCount = 4;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel1.Controls.Add(this.nameLabel, 0, 0);
			this.tableLayoutPanel1.Controls.Add(this.nameInput, 1, 0);
			this.tableLayoutPanel1.Controls.Add(this.dataLabel, 0, 1);
			this.tableLayoutPanel1.Controls.Add(this.dataInput, 1, 1);
			this.tableLayoutPanel1.Controls.Add(this.cancelButton, 0, 2);
			this.tableLayoutPanel1.Controls.Add(this.saveButton, 2, 2);
			this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
			this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(4);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 3;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(318, 106);
			// 
			// nameLabel
			// 
			this.nameLabel.AutoSize = true;
			this.nameLabel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.nameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.nameLabel.Location = new System.Drawing.Point(4, 0);
			this.nameLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.nameLabel.Name = "nameLabel";
			this.nameLabel.Size = new System.Drawing.Size(71, 35);
			this.nameLabel.Text = "Name:";
			this.nameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// nameInput
			// 
			this.nameInput.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.tableLayoutPanel1.SetColumnSpan(this.nameInput, 3);
			this.nameInput.Location = new System.Drawing.Point(82, 6);
			this.nameInput.Name = "nameInput";
			this.nameInput.Size = new System.Drawing.Size(233, 22);
			this.nameInput.TabIndex = 0;
			// 
			// dataLabel
			// 
			this.dataLabel.AutoSize = true;
			this.dataLabel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.dataLabel.Location = new System.Drawing.Point(3, 35);
			this.dataLabel.Name = "dataLabel";
			this.dataLabel.Size = new System.Drawing.Size(73, 35);
			this.dataLabel.Text = "Data:";
			this.dataLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// dataInput
			// 
			this.dataInput.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.tableLayoutPanel1.SetColumnSpan(this.dataInput, 3);
			this.dataInput.Location = new System.Drawing.Point(82, 41);
			this.dataInput.Name = "dataInput";
			this.dataInput.Size = new System.Drawing.Size(233, 22);
			this.dataInput.TabIndex = 1;
			// 
			// cancelButton
			// 
			this.tableLayoutPanel1.SetColumnSpan(this.cancelButton, 2);
			this.cancelButton.Dock = System.Windows.Forms.DockStyle.Fill;
			this.cancelButton.Location = new System.Drawing.Point(3, 73);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(152, 30);
			this.cancelButton.TabIndex = 2;
			this.cancelButton.Text = "Canel";
			this.cancelButton.UseVisualStyleBackColor = true;
			this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
			// 
			// saveButton
			// 
			this.tableLayoutPanel1.SetColumnSpan(this.saveButton, 2);
			this.saveButton.Dock = System.Windows.Forms.DockStyle.Fill;
			this.saveButton.Location = new System.Drawing.Point(161, 73);
			this.saveButton.Name = "saveButton";
			this.saveButton.Size = new System.Drawing.Size(154, 30);
			this.saveButton.TabIndex = 3;
			this.saveButton.Text = "Save";
			this.saveButton.UseVisualStyleBackColor = true;
			this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
			// 
			// ChangeDataWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(318, 106);
			this.Controls.Add(this.tableLayoutPanel1);
			this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Margin = new System.Windows.Forms.Padding(4);
			this.Name = "ChangeDataWindow";
			this.Text = "Change Data";
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Label nameLabel;
		private System.Windows.Forms.TextBox nameInput;
		private System.Windows.Forms.Label dataLabel;
		private System.Windows.Forms.TextBox dataInput;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Button saveButton;
	}
}