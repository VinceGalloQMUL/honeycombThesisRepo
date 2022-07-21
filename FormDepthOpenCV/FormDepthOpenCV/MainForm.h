#pragma once
#include <Windows.h>
//#include <WinBase.h>
//#include <WinUser.h>
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>


namespace FormDepthOpenCV {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	void openDepth(const char* filepathname, float sizeDivision, const char *calibFilename, bool useCalibration);
	void setDetectionRange(float dist);
	/// <summary>
   /// Summary for MainForm
   /// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

















	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::TextBox^  txtFilename;
	private: System::Windows::Forms::Button^  cmdBrowseDepth;













	private: System::Windows::Forms::Button^  cmdOpenComparison;




	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ txtSizeDivision;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ txtRefDist;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txtCalibFilename;
	private: System::Windows::Forms::Button^  cmdBrowseCalib;
	private: System::Windows::Forms::CheckBox^  chkUseCalibration;




















































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
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->txtFilename = (gcnew System::Windows::Forms::TextBox());
			this->cmdBrowseDepth = (gcnew System::Windows::Forms::Button());
			this->cmdOpenComparison = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtSizeDivision = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txtRefDist = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txtCalibFilename = (gcnew System::Windows::Forms::TextBox());
			this->cmdBrowseCalib = (gcnew System::Windows::Forms::Button());
			this->chkUseCalibration = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// txtFilename
			// 
			this->txtFilename->Location = System::Drawing::Point(95, 33);
			this->txtFilename->Name = L"txtFilename";
			this->txtFilename->Size = System::Drawing::Size(487, 20);
			this->txtFilename->TabIndex = 26;
			// 
			// cmdBrowseDepth
			// 
			this->cmdBrowseDepth->Location = System::Drawing::Point(589, 33);
			this->cmdBrowseDepth->Name = L"cmdBrowseDepth";
			this->cmdBrowseDepth->Size = System::Drawing::Size(37, 23);
			this->cmdBrowseDepth->TabIndex = 27;
			this->cmdBrowseDepth->Text = L"...";
			this->cmdBrowseDepth->UseVisualStyleBackColor = true;
			this->cmdBrowseDepth->Click += gcnew System::EventHandler(this, &MainForm::cmdBrowseDepth_Click);
			// 
			// cmdOpenComparison
			// 
			this->cmdOpenComparison->Location = System::Drawing::Point(632, 33);
			this->cmdOpenComparison->Name = L"cmdOpenComparison";
			this->cmdOpenComparison->Size = System::Drawing::Size(75, 23);
			this->cmdOpenComparison->TabIndex = 28;
			this->cmdOpenComparison->Text = L"Open";
			this->cmdOpenComparison->UseVisualStyleBackColor = true;
			this->cmdOpenComparison->Click += gcnew System::EventHandler(this, &MainForm::cmdOpenComparison_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(13, 33);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(36, 13);
			this->label3->TabIndex = 32;
			this->label3->Text = L"Image";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(22, 131);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(67, 13);
			this->label1->TabIndex = 34;
			this->label1->Text = L"Size Division";
			// 
			// txtSizeDivision
			// 
			this->txtSizeDivision->Location = System::Drawing::Point(95, 131);
			this->txtSizeDivision->Name = L"txtSizeDivision";
			this->txtSizeDivision->Size = System::Drawing::Size(32, 20);
			this->txtSizeDivision->TabIndex = 35;
			this->txtSizeDivision->Text = L"2.5";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(448, 127);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(70, 13);
			this->label2->TabIndex = 37;
			this->label2->Text = L"Ref Dist (mm)";
			// 
			// txtRefDist
			// 
			this->txtRefDist->Location = System::Drawing::Point(533, 124);
			this->txtRefDist->Name = L"txtRefDist";
			this->txtRefDist->Size = System::Drawing::Size(49, 20);
			this->txtRefDist->TabIndex = 38;
			this->txtRefDist->Text = L"10";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(13, 70);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(56, 13);
			this->label4->TabIndex = 39;
			this->label4->Text = L"Calibration";
			// 
			// txtCalibFilename
			// 
			this->txtCalibFilename->Location = System::Drawing::Point(95, 70);
			this->txtCalibFilename->Name = L"txtCalibFilename";
			this->txtCalibFilename->Size = System::Drawing::Size(487, 20);
			this->txtCalibFilename->TabIndex = 40;
			this->txtCalibFilename->Text = L"D:\\Projects\\PhD\\Research\\CombFormation\\Stigmergy\\2021\\DepthGauge\\Calibrate\\calibr"
				L"ation.txt";
			// 
			// cmdBrowseCalib
			// 
			this->cmdBrowseCalib->Location = System::Drawing::Point(589, 66);
			this->cmdBrowseCalib->Name = L"cmdBrowseCalib";
			this->cmdBrowseCalib->Size = System::Drawing::Size(37, 23);
			this->cmdBrowseCalib->TabIndex = 41;
			this->cmdBrowseCalib->Text = L"...";
			this->cmdBrowseCalib->UseVisualStyleBackColor = true;
			this->cmdBrowseCalib->Click += gcnew System::EventHandler(this, &MainForm::cmdBrowseCalib_Click);
			// 
			// chkUseCalibration
			// 
			this->chkUseCalibration->AutoSize = true;
			this->chkUseCalibration->Location = System::Drawing::Point(95, 97);
			this->chkUseCalibration->Name = L"chkUseCalibration";
			this->chkUseCalibration->Size = System::Drawing::Size(97, 17);
			this->chkUseCalibration->TabIndex = 42;
			this->chkUseCalibration->Text = L"Use Calibration";
			this->chkUseCalibration->UseVisualStyleBackColor = true;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(729, 159);
			this->Controls->Add(this->chkUseCalibration);
			this->Controls->Add(this->cmdBrowseCalib);
			this->Controls->Add(this->txtCalibFilename);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->txtRefDist);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txtSizeDivision);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->cmdOpenComparison);
			this->Controls->Add(this->cmdBrowseDepth);
			this->Controls->Add(this->txtFilename);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"Depth calculator";
			this->Activated += gcnew System::EventHandler(this, &MainForm::MainForm_Activated);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	float sizeDivision;

	void MarshalString(String ^ s, std::string& os) {
		using namespace Runtime::InteropServices;
		const char* chars =
			(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}
	int MarshalStringToInt(String ^ s) {
		int lclInt = 0;
		std::string tmpStr;
		MarshalString(s, tmpStr);
		try {
			lclInt = std::stoi(tmpStr);
		}
		catch (...) {

		}
		return lclInt;
	}
	float MarshalStringToFloat(String ^ s) {
		float lclFloat = 0;
		std::string tmpStr;
		MarshalString(s, tmpStr);
		try {
			lclFloat = std::stof(tmpStr);
		}
		catch (...) {

		}
		return lclFloat;
	}

	private: System::Void MainForm_Activated(System::Object^  sender, System::EventArgs^  e) {
	}
			 bool mCOmpFile1Present = false;
			 bool mCOmpFilePreviousPresent = false;

	private: System::Void cmdOpenComparison_Click(System::Object^  sender, System::EventArgs^  e) {
		std::string tmpStr1;
		MarshalString(txtFilename->Text, tmpStr1);
		std::string tmpStr2;
		MarshalString(txtCalibFilename->Text, tmpStr2);
		sizeDivision = MarshalStringToFloat(txtSizeDivision->Text);
		openDepth(tmpStr1.c_str(), sizeDivision, tmpStr2.c_str(), chkUseCalibration->Checked);
		float rangeSq = MarshalStringToFloat(txtRefDist->Text);
		setDetectionRange(rangeSq);
	}
	private: System::Void cmdBrowseDepth_Click(System::Object^  sender, System::EventArgs^  e) {
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			txtFilename->Text = openFileDialog1->FileName;
			mCOmpFile1Present = true;
		}
	}
private: System::Void txtSizeDivision_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void cmdBrowseCalib_Click(System::Object^  sender, System::EventArgs^  e) {
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			txtCalibFilename->Text = openFileDialog1->FileName;
		}
}
};
}