#pragma once
#include <Windows.h>
//#include <WinBase.h>
//#include <WinUser.h>
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>


namespace FormImageCompareOpenCV {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	int imageH;
	int imageW;
	int mGeometryFrameRef = -1;
	int mGeometryFrameStart = -1;
	int mGeometryFrameEnd = -1;

	void setGeoTypesShown(bool *setValsNative, bool *setValsRandom, int arrySize);
	int getWidth();
	int getHeight();
	int getFrameNum();
	int openVideo(char* filepathname);
	void showNVideoFrame(int num, bool applyTransform);
	void showOneVideoFrame(bool applyTransform);
	std::string beeDisplaySave(char* filepathname);
	void clearAverage();
	float getFrameRate();
	void setVideoFrame(int relativeFrame);
	void setVideoFrameAbs(int absFrame);
	void openComparison(const char* filepathname);
	void openComparison(const char* filepathname, const char* filepathname2, const char *filename1, const char * filename2, float sizeDivision);
	void openTest(const char* filepathname, const char *filename, float sizeDivision);
	void openReference(const char* filepathname, int frameNUmber);
	void setGeometryUsingFrame(int frameNo, int RefStartOrEnd);
	void forceFullSize(bool doFullsize);
	void setScaleDistanceMM(float dist);
	float getScaleDistanceMM();
	std::string getHive();
	void setHive(std::string hive);
	void saveConfigFile();
	std::string getCellData(std::string boundaryType);
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










	private: System::Windows::Forms::TextBox^  txtFilenameComparison;

	private: System::Windows::Forms::Button^  cmdBrowseComparison;
	private: System::Windows::Forms::Button^  cmdOpenComparison;


	private: System::Windows::Forms::TextBox^  txtFilenameComparison2nd;
	private: System::Windows::Forms::Button^  cmdBrowseComparison2nd;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ txtSizeDivision;
	private: System::Windows::Forms::Button^ cmdSetRefDist;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ txtRefDist;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TextBox^ txtHive;
	private: System::Windows::Forms::CheckBox^  chkWallEnd;
	private: System::Windows::Forms::CheckBox^  chkStraight;
	private: System::Windows::Forms::CheckBox^  chkBent;
	private: System::Windows::Forms::CheckBox^  chkPitGap;
	private: System::Windows::Forms::CheckBox^  chkBulge;
	private: System::Windows::Forms::CheckBox^  chkBulge2;
	private: System::Windows::Forms::CheckBox^  chkRimPit;
	private: System::Windows::Forms::CheckBox^  chkRim2Pits;
	private: System::Windows::Forms::CheckBox^  chkPitCell;
	private: System::Windows::Forms::CheckBox^  chkPitCell2;
	private: System::Windows::Forms::CheckBox^  chkRandoms;
	private: System::Windows::Forms::Button^  cmdSetAll;
	private: System::Windows::Forms::Button^  cmdClearAll;
	private: System::Windows::Forms::Button^  cmdSetShown;
	private: System::Windows::Forms::Button^  cmdTesting;
	private: System::Windows::Forms::Button^  cmdSetGetCellData;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  txtCellTypes;
	private: System::Windows::Forms::TextBox^  txtCellData;
	private: System::Windows::Forms::CheckBox^  chkRnd2Pits;
	private: System::Windows::Forms::CheckBox^  chkRndBent;































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
			this->txtFilenameComparison = (gcnew System::Windows::Forms::TextBox());
			this->cmdBrowseComparison = (gcnew System::Windows::Forms::Button());
			this->cmdOpenComparison = (gcnew System::Windows::Forms::Button());
			this->txtFilenameComparison2nd = (gcnew System::Windows::Forms::TextBox());
			this->cmdBrowseComparison2nd = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtSizeDivision = (gcnew System::Windows::Forms::TextBox());
			this->cmdSetRefDist = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txtRefDist = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->txtHive = (gcnew System::Windows::Forms::TextBox());
			this->chkWallEnd = (gcnew System::Windows::Forms::CheckBox());
			this->chkStraight = (gcnew System::Windows::Forms::CheckBox());
			this->chkBent = (gcnew System::Windows::Forms::CheckBox());
			this->chkPitGap = (gcnew System::Windows::Forms::CheckBox());
			this->chkBulge = (gcnew System::Windows::Forms::CheckBox());
			this->chkBulge2 = (gcnew System::Windows::Forms::CheckBox());
			this->chkRimPit = (gcnew System::Windows::Forms::CheckBox());
			this->chkRim2Pits = (gcnew System::Windows::Forms::CheckBox());
			this->chkPitCell = (gcnew System::Windows::Forms::CheckBox());
			this->chkPitCell2 = (gcnew System::Windows::Forms::CheckBox());
			this->chkRandoms = (gcnew System::Windows::Forms::CheckBox());
			this->cmdSetAll = (gcnew System::Windows::Forms::Button());
			this->cmdClearAll = (gcnew System::Windows::Forms::Button());
			this->cmdSetShown = (gcnew System::Windows::Forms::Button());
			this->cmdTesting = (gcnew System::Windows::Forms::Button());
			this->cmdSetGetCellData = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->txtCellTypes = (gcnew System::Windows::Forms::TextBox());
			this->txtCellData = (gcnew System::Windows::Forms::TextBox());
			this->chkRnd2Pits = (gcnew System::Windows::Forms::CheckBox());
			this->chkRndBent = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// txtFilenameComparison
			// 
			this->txtFilenameComparison->Location = System::Drawing::Point(95, 33);
			this->txtFilenameComparison->Name = L"txtFilenameComparison";
			this->txtFilenameComparison->Size = System::Drawing::Size(487, 20);
			this->txtFilenameComparison->TabIndex = 26;
			// 
			// cmdBrowseComparison
			// 
			this->cmdBrowseComparison->Location = System::Drawing::Point(589, 29);
			this->cmdBrowseComparison->Name = L"cmdBrowseComparison";
			this->cmdBrowseComparison->Size = System::Drawing::Size(37, 23);
			this->cmdBrowseComparison->TabIndex = 27;
			this->cmdBrowseComparison->Text = L"...";
			this->cmdBrowseComparison->UseVisualStyleBackColor = true;
			this->cmdBrowseComparison->Click += gcnew System::EventHandler(this, &MainForm::cmdBrowseComparison_Click);
			// 
			// cmdOpenComparison
			// 
			this->cmdOpenComparison->Location = System::Drawing::Point(632, 57);
			this->cmdOpenComparison->Name = L"cmdOpenComparison";
			this->cmdOpenComparison->Size = System::Drawing::Size(75, 23);
			this->cmdOpenComparison->TabIndex = 28;
			this->cmdOpenComparison->Text = L"Open";
			this->cmdOpenComparison->UseVisualStyleBackColor = true;
			this->cmdOpenComparison->Click += gcnew System::EventHandler(this, &MainForm::cmdOpenComparison_Click);
			// 
			// txtFilenameComparison2nd
			// 
			this->txtFilenameComparison2nd->Location = System::Drawing::Point(95, 59);
			this->txtFilenameComparison2nd->Name = L"txtFilenameComparison2nd";
			this->txtFilenameComparison2nd->Size = System::Drawing::Size(487, 20);
			this->txtFilenameComparison2nd->TabIndex = 30;
			// 
			// cmdBrowseComparison2nd
			// 
			this->cmdBrowseComparison2nd->Location = System::Drawing::Point(589, 56);
			this->cmdBrowseComparison2nd->Name = L"cmdBrowseComparison2nd";
			this->cmdBrowseComparison2nd->Size = System::Drawing::Size(37, 23);
			this->cmdBrowseComparison2nd->TabIndex = 31;
			this->cmdBrowseComparison2nd->Text = L"...";
			this->cmdBrowseComparison2nd->UseVisualStyleBackColor = true;
			this->cmdBrowseComparison2nd->Click += gcnew System::EventHandler(this, &MainForm::cmdBrowseComparison2nd_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(13, 33);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(76, 13);
			this->label3->TabIndex = 32;
			this->label3->Text = L"Image0 (static)";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(13, 59);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(81, 13);
			this->label4->TabIndex = 33;
			this->label4->Text = L"Image1 (mobile)";
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
			this->txtSizeDivision->TextChanged += gcnew System::EventHandler(this, &MainForm::txtSizeDivision_TextChanged);
			// 
			// cmdSetRefDist
			// 
			this->cmdSetRefDist->Location = System::Drawing::Point(632, 153);
			this->cmdSetRefDist->Name = L"cmdSetRefDist";
			this->cmdSetRefDist->Size = System::Drawing::Size(75, 23);
			this->cmdSetRefDist->TabIndex = 36;
			this->cmdSetRefDist->Text = L"Set n Save";
			this->cmdSetRefDist->UseVisualStyleBackColor = true;
			this->cmdSetRefDist->Click += gcnew System::EventHandler(this, &MainForm::cmdSetRefDist_Click);
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
			this->txtRefDist->Text = L"115";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(451, 155);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(29, 13);
			this->label5->TabIndex = 39;
			this->label5->Text = L"Hive";
			// 
			// txtHive
			// 
			this->txtHive->Location = System::Drawing::Point(533, 155);
			this->txtHive->Name = L"txtHive";
			this->txtHive->Size = System::Drawing::Size(49, 20);
			this->txtHive->TabIndex = 40;
			this->txtHive->Text = L"X";
			// 
			// chkWallEnd
			// 
			this->chkWallEnd->AutoSize = true;
			this->chkWallEnd->Checked = true;
			this->chkWallEnd->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkWallEnd->Location = System::Drawing::Point(95, 195);
			this->chkWallEnd->Name = L"chkWallEnd";
			this->chkWallEnd->Size = System::Drawing::Size(66, 17);
			this->chkWallEnd->TabIndex = 51;
			this->chkWallEnd->Text = L"WallEnd";
			this->chkWallEnd->UseVisualStyleBackColor = true;
			// 
			// chkStraight
			// 
			this->chkStraight->AutoSize = true;
			this->chkStraight->Checked = true;
			this->chkStraight->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkStraight->Location = System::Drawing::Point(95, 212);
			this->chkStraight->Name = L"chkStraight";
			this->chkStraight->Size = System::Drawing::Size(62, 17);
			this->chkStraight->TabIndex = 52;
			this->chkStraight->Text = L"Straight";
			this->chkStraight->UseVisualStyleBackColor = true;
			// 
			// chkBent
			// 
			this->chkBent->AutoSize = true;
			this->chkBent->Checked = true;
			this->chkBent->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkBent->Location = System::Drawing::Point(95, 229);
			this->chkBent->Name = L"chkBent";
			this->chkBent->Size = System::Drawing::Size(48, 17);
			this->chkBent->TabIndex = 53;
			this->chkBent->Text = L"Bent";
			this->chkBent->UseVisualStyleBackColor = true;
			// 
			// chkPitGap
			// 
			this->chkPitGap->AutoSize = true;
			this->chkPitGap->Checked = true;
			this->chkPitGap->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkPitGap->Location = System::Drawing::Point(95, 246);
			this->chkPitGap->Name = L"chkPitGap";
			this->chkPitGap->Size = System::Drawing::Size(58, 17);
			this->chkPitGap->TabIndex = 54;
			this->chkPitGap->Text = L"PitGap";
			this->chkPitGap->UseVisualStyleBackColor = true;
			// 
			// chkBulge
			// 
			this->chkBulge->AutoSize = true;
			this->chkBulge->Checked = true;
			this->chkBulge->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkBulge->Location = System::Drawing::Point(95, 263);
			this->chkBulge->Name = L"chkBulge";
			this->chkBulge->Size = System::Drawing::Size(53, 17);
			this->chkBulge->TabIndex = 55;
			this->chkBulge->Text = L"Bulge";
			this->chkBulge->UseVisualStyleBackColor = true;
			// 
			// chkBulge2
			// 
			this->chkBulge2->AutoSize = true;
			this->chkBulge2->Checked = true;
			this->chkBulge2->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkBulge2->Location = System::Drawing::Point(95, 280);
			this->chkBulge2->Name = L"chkBulge2";
			this->chkBulge2->Size = System::Drawing::Size(59, 17);
			this->chkBulge2->TabIndex = 56;
			this->chkBulge2->Text = L"Bulge2";
			this->chkBulge2->UseVisualStyleBackColor = true;
			// 
			// chkRimPit
			// 
			this->chkRimPit->AutoSize = true;
			this->chkRimPit->Checked = true;
			this->chkRimPit->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkRimPit->Location = System::Drawing::Point(95, 297);
			this->chkRimPit->Name = L"chkRimPit";
			this->chkRimPit->Size = System::Drawing::Size(56, 17);
			this->chkRimPit->TabIndex = 57;
			this->chkRimPit->Text = L"RimPit";
			this->chkRimPit->UseVisualStyleBackColor = true;
			// 
			// chkRim2Pits
			// 
			this->chkRim2Pits->AutoSize = true;
			this->chkRim2Pits->Checked = true;
			this->chkRim2Pits->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkRim2Pits->Location = System::Drawing::Point(95, 314);
			this->chkRim2Pits->Name = L"chkRim2Pits";
			this->chkRim2Pits->Size = System::Drawing::Size(67, 17);
			this->chkRim2Pits->TabIndex = 58;
			this->chkRim2Pits->Text = L"Rim2Pits";
			this->chkRim2Pits->UseVisualStyleBackColor = true;
			// 
			// chkPitCell
			// 
			this->chkPitCell->AutoSize = true;
			this->chkPitCell->Checked = true;
			this->chkPitCell->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkPitCell->Location = System::Drawing::Point(95, 331);
			this->chkPitCell->Name = L"chkPitCell";
			this->chkPitCell->Size = System::Drawing::Size(55, 17);
			this->chkPitCell->TabIndex = 59;
			this->chkPitCell->Text = L"PitCell";
			this->chkPitCell->UseVisualStyleBackColor = true;
			// 
			// chkPitCell2
			// 
			this->chkPitCell2->AutoSize = true;
			this->chkPitCell2->Checked = true;
			this->chkPitCell2->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkPitCell2->Location = System::Drawing::Point(95, 348);
			this->chkPitCell2->Name = L"chkPitCell2";
			this->chkPitCell2->Size = System::Drawing::Size(61, 17);
			this->chkPitCell2->TabIndex = 60;
			this->chkPitCell2->Text = L"PitCell2";
			this->chkPitCell2->UseVisualStyleBackColor = true;
			// 
			// chkRandoms
			// 
			this->chkRandoms->AutoSize = true;
			this->chkRandoms->Checked = true;
			this->chkRandoms->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkRandoms->Location = System::Drawing::Point(177, 297);
			this->chkRandoms->Name = L"chkRandoms";
			this->chkRandoms->Size = System::Drawing::Size(15, 14);
			this->chkRandoms->TabIndex = 61;
			this->chkRandoms->UseVisualStyleBackColor = true;
			// 
			// cmdSetAll
			// 
			this->cmdSetAll->Location = System::Drawing::Point(95, 166);
			this->cmdSetAll->Name = L"cmdSetAll";
			this->cmdSetAll->Size = System::Drawing::Size(75, 23);
			this->cmdSetAll->TabIndex = 62;
			this->cmdSetAll->Text = L"Tick";
			this->cmdSetAll->UseVisualStyleBackColor = true;
			this->cmdSetAll->Click += gcnew System::EventHandler(this, &MainForm::cmdSetAll_Click);
			// 
			// cmdClearAll
			// 
			this->cmdClearAll->Location = System::Drawing::Point(19, 166);
			this->cmdClearAll->Name = L"cmdClearAll";
			this->cmdClearAll->Size = System::Drawing::Size(75, 23);
			this->cmdClearAll->TabIndex = 63;
			this->cmdClearAll->Text = L"Untick";
			this->cmdClearAll->UseVisualStyleBackColor = true;
			this->cmdClearAll->Click += gcnew System::EventHandler(this, &MainForm::cmdClearAll_Click);
			// 
			// cmdSetShown
			// 
			this->cmdSetShown->Location = System::Drawing::Point(95, 372);
			this->cmdSetShown->Name = L"cmdSetShown";
			this->cmdSetShown->Size = System::Drawing::Size(75, 23);
			this->cmdSetShown->TabIndex = 64;
			this->cmdSetShown->Text = L"Set";
			this->cmdSetShown->UseVisualStyleBackColor = true;
			this->cmdSetShown->Click += gcnew System::EventHandler(this, &MainForm::cmdSetShown_Click);
			// 
			// cmdTesting
			// 
			this->cmdTesting->Location = System::Drawing::Point(632, 28);
			this->cmdTesting->Name = L"cmdTesting";
			this->cmdTesting->Size = System::Drawing::Size(75, 23);
			this->cmdTesting->TabIndex = 65;
			this->cmdTesting->Text = L"Testing";
			this->cmdTesting->UseVisualStyleBackColor = true;
			this->cmdTesting->Click += gcnew System::EventHandler(this, &MainForm::cmdTesting_Click);
			// 
			// cmdSetGetCellData
			// 
			this->cmdSetGetCellData->Location = System::Drawing::Point(632, 200);
			this->cmdSetGetCellData->Name = L"cmdSetGetCellData";
			this->cmdSetGetCellData->Size = System::Drawing::Size(75, 23);
			this->cmdSetGetCellData->TabIndex = 66;
			this->cmdSetGetCellData->Text = L"SetGet";
			this->cmdSetGetCellData->UseVisualStyleBackColor = true;
			this->cmdSetGetCellData->Click += gcnew System::EventHandler(this, &MainForm::cmdSetGetCellData_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(451, 184);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(47, 13);
			this->label6->TabIndex = 67;
			this->label6->Text = L"CellData";
			// 
			// txtCellTypes
			// 
			this->txtCellTypes->Location = System::Drawing::Point(533, 181);
			this->txtCellTypes->Name = L"txtCellTypes";
			this->txtCellTypes->Size = System::Drawing::Size(174, 20);
			this->txtCellTypes->TabIndex = 68;
			this->txtCellTypes->Text = L"DD,2749,1120,2772,1517";
			// 
			// txtCellData
			// 
			this->txtCellData->Location = System::Drawing::Point(533, 229);
			this->txtCellData->Multiline = true;
			this->txtCellData->Name = L"txtCellData";
			this->txtCellData->Size = System::Drawing::Size(174, 188);
			this->txtCellData->TabIndex = 69;
			// 
			// chkRnd2Pits
			// 
			this->chkRnd2Pits->AutoSize = true;
			this->chkRnd2Pits->Location = System::Drawing::Point(177, 314);
			this->chkRnd2Pits->Name = L"chkRnd2Pits";
			this->chkRnd2Pits->Size = System::Drawing::Size(15, 14);
			this->chkRnd2Pits->TabIndex = 70;
			this->chkRnd2Pits->UseVisualStyleBackColor = true;
			// 
			// chkRndBent
			// 
			this->chkRndBent->AutoSize = true;
			this->chkRndBent->Location = System::Drawing::Point(177, 231);
			this->chkRndBent->Name = L"chkRndBent";
			this->chkRndBent->Size = System::Drawing::Size(71, 17);
			this->chkRndBent->TabIndex = 71;
			this->chkRndBent->Text = L"Randoms";
			this->chkRndBent->UseVisualStyleBackColor = true;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(729, 429);
			this->Controls->Add(this->chkRndBent);
			this->Controls->Add(this->chkRnd2Pits);
			this->Controls->Add(this->txtCellData);
			this->Controls->Add(this->txtCellTypes);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->cmdSetGetCellData);
			this->Controls->Add(this->cmdTesting);
			this->Controls->Add(this->cmdSetShown);
			this->Controls->Add(this->cmdClearAll);
			this->Controls->Add(this->cmdSetAll);
			this->Controls->Add(this->chkRandoms);
			this->Controls->Add(this->chkPitCell2);
			this->Controls->Add(this->chkPitCell);
			this->Controls->Add(this->chkRim2Pits);
			this->Controls->Add(this->chkRimPit);
			this->Controls->Add(this->chkBulge2);
			this->Controls->Add(this->chkBulge);
			this->Controls->Add(this->chkPitGap);
			this->Controls->Add(this->chkBent);
			this->Controls->Add(this->chkStraight);
			this->Controls->Add(this->chkWallEnd);
			this->Controls->Add(this->txtHive);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->txtRefDist);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->cmdSetRefDist);
			this->Controls->Add(this->txtSizeDivision);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->cmdBrowseComparison2nd);
			this->Controls->Add(this->txtFilenameComparison2nd);
			this->Controls->Add(this->cmdOpenComparison);
			this->Controls->Add(this->cmdBrowseComparison);
			this->Controls->Add(this->txtFilenameComparison);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"Prepare sparse composite image";
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
			 bool mCOmpFile2Present = false;

	private: System::Void cmdOpenComparison_Click(System::Object^  sender, System::EventArgs^  e) {
		std::string tmpStr2;
		std::string tmpStr1;
		if (mCOmpFile2Present) {
			MarshalString(txtFilenameComparison2nd->Text, tmpStr2);
			 if (mCOmpFile1Present) {
				MarshalString(txtFilenameComparison->Text, tmpStr1);
				std::string base_filename1 = tmpStr1.substr(tmpStr1.find_last_of("/\\") + 1);
				std::string base_filename2 = tmpStr2.substr(tmpStr2.find_last_of("/\\") + 1);
				sizeDivision = MarshalStringToFloat(txtSizeDivision->Text);
				openComparison(tmpStr2.c_str(), tmpStr1.c_str(), base_filename1.c_str(), base_filename2.c_str(), sizeDivision);
				float refDist = getScaleDistanceMM();
				txtRefDist->Text = refDist.ToString();
				std::string hive  = getHive();
				txtHive->Text = gcnew String(hive.c_str());
			}
		}
	}
	private: System::Void cmdBrowseComparison_Click(System::Object^  sender, System::EventArgs^  e) {
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			txtFilenameComparison->Text = openFileDialog1->FileName;
			mCOmpFile1Present = true;
		}
	}
	private: System::Void cmdBrowseComparison2nd_Click(System::Object^  sender, System::EventArgs^  e) {
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			txtFilenameComparison2nd->Text = openFileDialog1->FileName;
			mCOmpFile2Present = true;
		}
	}
private: System::Void cmdSetRefDist_Click(System::Object^ sender, System::EventArgs^ e) {
	std::string hive;
	MarshalString(txtHive->Text, hive);
	float refDist = MarshalStringToFloat(txtRefDist->Text);
	setScaleDistanceMM(refDist);
	setHive(hive);
	saveConfigFile();
}
private: System::Void txtSizeDivision_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void cmdSetClearAll(bool state) {
	chkBent->Checked = state;
	chkBulge->Checked = state;
	chkBulge2->Checked = state;
	chkPitCell->Checked = state;
	chkPitCell2->Checked = state;
	chkPitGap->Checked = state;
	chkRim2Pits->Checked = state;
	chkRimPit->Checked = state;
	chkStraight->Checked = state;
	chkWallEnd->Checked = state;
	chkRnd2Pits->Checked = state;
	chkRndBent->Checked = state;
	chkRandoms->Checked = state;
}
private: System::Void cmdClearAll_Click(System::Object^  sender, System::EventArgs^  e) {
	cmdSetClearAll(false);
}
private: System::Void cmdSetAll_Click(System::Object^  sender, System::EventArgs^  e) {
	cmdSetClearAll(true);
}
private: System::Void cmdSetShown_Click(System::Object^  sender, System::EventArgs^  e) {
	bool showArr[11];
	bool showArrR[11];
	showArr[0] = false;
	showArr[1] = chkWallEnd->Checked;
	showArr[2] = chkStraight->Checked;
	showArr[3] = chkBent->Checked;
	showArr[4] = chkPitGap->Checked;
	showArr[5] = chkBulge->Checked;
	showArr[6] = chkBulge2->Checked;
	showArr[7] = chkRimPit->Checked;
	showArr[8] = chkRim2Pits->Checked;
	showArr[9] = chkPitCell->Checked;
	showArr[10] = chkPitCell2->Checked;
	showArrR[0] = false;
	showArrR[1] = false;
	showArrR[2] = false;
	showArrR[3] = chkRndBent->Checked;
	showArrR[4] = false;
	showArrR[5] = false;
	showArrR[6] = false;
	showArrR[7] = chkRandoms->Checked;
	showArrR[8] = chkRnd2Pits->Checked;
	showArrR[9] = false;
	showArrR[10] = false;
	setGeoTypesShown(&showArr[0], &showArrR[0], sizeof(showArr));
}
private: System::Void cmdTesting_Click(System::Object^  sender, System::EventArgs^  e) {
	std::string tmpStr1;
	MarshalString(txtFilenameComparison->Text, tmpStr1);
	if (tmpStr1.length() > 0) {
		std::string base_filename1 = tmpStr1.substr(tmpStr1.find_last_of("/\\") + 1);
		sizeDivision = MarshalStringToFloat(txtSizeDivision->Text);
		openTest(tmpStr1.c_str(), base_filename1.c_str(), sizeDivision);
		float refDist = getScaleDistanceMM();
		txtRefDist->Text = refDist.ToString();
	}

}
private: System::Void cmdSetGetCellData_Click(System::Object^  sender, System::EventArgs^  e) {
	std::string tmpStr1;
	MarshalString(txtCellTypes->Text, tmpStr1);
	tmpStr1 = getCellData(tmpStr1);
	txtCellData->Text = gcnew String(tmpStr1.c_str());
}
};
}