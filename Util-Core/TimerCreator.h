#pragma once

#include "src\timer\Timer.h"
#include "src\Futils.h"

namespace futils {

	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for TimerCreator
	/// </summary>
	public ref class TimerCreator : public System::Windows::Forms::Form
	{
	private:
		time_t m_Time;
		System::String^ m_Name;
	public:
		static futils::Timer CreateTimer()
		{
			TimerCreator^ obj = gcnew TimerCreator();
			obj->ShowDialog();
			return futils::Timer(to_CPP_String(obj->m_Name), obj->m_Time);
		}
	protected:
		TimerCreator(void)
			: m_Time(0)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TimerCreator()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  m_Layout;
	protected:

	private: System::Windows::Forms::DateTimePicker^  m_DatePicker;
	private: System::Windows::Forms::Label^  m_DateLabel;

	private: System::Windows::Forms::DateTimePicker^  m_TimePicker;
	private: System::Windows::Forms::Button^  m_CancelButton;
	private: System::Windows::Forms::Button^  m_CreateButton;
	private: System::Windows::Forms::Label^  m_TimeLabel;
	private: System::Windows::Forms::Label^  m_NameLabel;
	private: System::Windows::Forms::TextBox^  m_NameInput;



	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->m_Layout = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->m_TimeLabel = (gcnew System::Windows::Forms::Label());
			this->m_DateLabel = (gcnew System::Windows::Forms::Label());
			this->m_CancelButton = (gcnew System::Windows::Forms::Button());
			this->m_CreateButton = (gcnew System::Windows::Forms::Button());
			this->m_NameLabel = (gcnew System::Windows::Forms::Label());
			this->m_DatePicker = (gcnew System::Windows::Forms::DateTimePicker());
			this->m_TimePicker = (gcnew System::Windows::Forms::DateTimePicker());
			this->m_NameInput = (gcnew System::Windows::Forms::TextBox());
			this->m_Layout->SuspendLayout();
			this->SuspendLayout();
			// 
			// m_Layout
			// 
			this->m_Layout->ColumnCount = 2;
			this->m_Layout->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->m_Layout->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->m_Layout->Controls->Add(this->m_DateLabel, 0, 1);
			this->m_Layout->Controls->Add(this->m_CancelButton, 0, 3);
			this->m_Layout->Controls->Add(this->m_CreateButton, 1, 3);
			this->m_Layout->Controls->Add(this->m_NameLabel, 0, 0);
			this->m_Layout->Controls->Add(this->m_NameInput, 1, 0);
			this->m_Layout->Controls->Add(this->m_DatePicker, 1, 1);
			this->m_Layout->Controls->Add(this->m_TimeLabel, 0, 2);
			this->m_Layout->Controls->Add(this->m_TimePicker, 1, 2);
			this->m_Layout->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_Layout->Location = System::Drawing::Point(0, 0);
			this->m_Layout->Margin = System::Windows::Forms::Padding(4);
			this->m_Layout->Name = L"m_Layout";
			this->m_Layout->RowCount = 4;
			this->m_Layout->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25)));
			this->m_Layout->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25)));
			this->m_Layout->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25)));
			this->m_Layout->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25)));
			this->m_Layout->Size = System::Drawing::Size(353, 112);
			this->m_Layout->TabIndex = 0;
			// 
			// m_TimeLabel
			// 
			this->m_TimeLabel->AutoSize = true;
			this->m_TimeLabel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_TimeLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->m_TimeLabel->Location = System::Drawing::Point(3, 56);
			this->m_TimeLabel->Name = L"m_TimeLabel";
			this->m_TimeLabel->Size = System::Drawing::Size(170, 28);
			this->m_TimeLabel->TabIndex = 7;
			this->m_TimeLabel->Text = L"Time:";
			this->m_TimeLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// m_DateLabel
			// 
			this->m_DateLabel->AutoSize = true;
			this->m_DateLabel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_DateLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->m_DateLabel->Location = System::Drawing::Point(3, 28);
			this->m_DateLabel->Name = L"m_DateLabel";
			this->m_DateLabel->Size = System::Drawing::Size(170, 28);
			this->m_DateLabel->TabIndex = 1;
			this->m_DateLabel->Text = L"Date:";
			this->m_DateLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// m_CancelButton
			// 
			this->m_CancelButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_CancelButton->Location = System::Drawing::Point(3, 87);
			this->m_CancelButton->Name = L"m_CancelButton";
			this->m_CancelButton->Size = System::Drawing::Size(170, 22);
			this->m_CancelButton->TabIndex = 4;
			this->m_CancelButton->Text = L"Cancel";
			this->m_CancelButton->UseVisualStyleBackColor = true;
			this->m_CancelButton->Click += gcnew System::EventHandler(this, &TimerCreator::m_CancelButton_Click);
			// 
			// m_CreateButton
			// 
			this->m_CreateButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_CreateButton->Location = System::Drawing::Point(179, 87);
			this->m_CreateButton->Name = L"m_CreateButton";
			this->m_CreateButton->Size = System::Drawing::Size(171, 22);
			this->m_CreateButton->TabIndex = 5;
			this->m_CreateButton->Text = L"Create";
			this->m_CreateButton->UseVisualStyleBackColor = true;
			this->m_CreateButton->Click += gcnew System::EventHandler(this, &TimerCreator::m_CreateButton_Click);
			// 
			// m_NameLabel
			// 
			this->m_NameLabel->AutoSize = true;
			this->m_NameLabel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_NameLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->m_NameLabel->Location = System::Drawing::Point(3, 0);
			this->m_NameLabel->Name = L"m_NameLabel";
			this->m_NameLabel->Size = System::Drawing::Size(170, 28);
			this->m_NameLabel->TabIndex = 6;
			this->m_NameLabel->Text = L"Name:";
			this->m_NameLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// m_DatePicker
			// 
			this->m_DatePicker->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->m_DatePicker->Format = System::Windows::Forms::DateTimePickerFormat::Short;
			this->m_DatePicker->Location = System::Drawing::Point(179, 31);
			this->m_DatePicker->MinDate = System::DateTime(2000, 1, 1, 0, 0, 0, 0);
			this->m_DatePicker->Name = L"m_DatePicker";
			this->m_DatePicker->Size = System::Drawing::Size(171, 22);
			this->m_DatePicker->TabIndex = 0;
			// 
			// m_TimePicker
			// 
			this->m_TimePicker->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->m_TimePicker->Format = System::Windows::Forms::DateTimePickerFormat::Time;
			this->m_TimePicker->Location = System::Drawing::Point(179, 59);
			this->m_TimePicker->Name = L"m_TimePicker";
			this->m_TimePicker->Size = System::Drawing::Size(171, 22);
			this->m_TimePicker->TabIndex = 3;
			// 
			// m_NameInput
			// 
			this->m_NameInput->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->m_NameInput->Location = System::Drawing::Point(179, 3);
			this->m_NameInput->Name = L"m_NameInput";
			this->m_NameInput->Size = System::Drawing::Size(171, 22);
			this->m_NameInput->TabIndex = 8;
			// 
			// TimerCreator
			// 
			this->AcceptButton = this->m_CreateButton;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(353, 112);
			this->Controls->Add(this->m_Layout);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"TimerCreator";
			this->Text = L"TimerCreator";
			this->m_Layout->ResumeLayout(false);
			this->m_Layout->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void m_CancelButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Close();
	}
	private: System::Void m_CreateButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		System::DateTime^ t = gcnew System::DateTime(m_DatePicker->Value.Ticks);
		t->AddHours(m_TimePicker->Value.Hour);
		t->AddMinutes(m_TimePicker->Value.Minute);
		t->AddSeconds(m_TimePicker->Value.Second);
		m_Time = static_cast<time_t>((t->ToUniversalTime() - System::DateTime(1970, 1, 1)).TotalSeconds);

		m_Name = m_NameInput->Text;

		Close();
	}
};
}
