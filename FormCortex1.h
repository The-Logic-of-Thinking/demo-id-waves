#include "Brain.h"

#include "FormCortex2.h"

using namespace System;
using namespace System::Windows::Forms;


namespace VA {
	public ref class Form1 : public System::Windows::Forms::Form
	{
		public:
			Form1(void)
			{
				InitializeComponent();
			}

		protected:
			~Form1()
			{
				if (components)
				{
					delete components;
				}
			}
		
		private: System::Windows::Forms::Button^  Button1;
		private: System::Windows::Forms::Button^  Button11;
		private: System::Windows::Forms::Button^  Button5;
		private: System::Windows::Forms::Label^  Label1;
		private: System::Windows::Forms::Button^  Button3;
		private: System::Windows::Forms::CheckedListBox^  CheckedListBox1;
		private: System::Windows::Forms::Button^  Button6;
		private: System::Windows::Forms::Label^  Label3;
		private: System::Windows::Forms::Label^  Label2;
		private: System::Windows::Forms::Button^  Button2;
		private: System::Windows::Forms::Button^  Button4;
		private: System::Windows::Forms::ComboBox^  ComboBox2;
		private: System::Windows::Forms::PictureBox^  PictureBox4;
		private: System::Windows::Forms::PictureBox^  PictureBox2;
		private: System::Windows::Forms::ComboBox^  ComboBox1;
		private: System::Windows::Forms::PictureBox^  PictureBox3;
		private: System::Windows::Forms::PictureBox^  PictureBox1;
		private: System::Windows::Forms::Label^  label4;
		private: System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
			void InitializeComponent(void)
			{
				this->Button1 = (gcnew System::Windows::Forms::Button());
				this->Button11 = (gcnew System::Windows::Forms::Button());
				this->Button5 = (gcnew System::Windows::Forms::Button());
				this->Label1 = (gcnew System::Windows::Forms::Label());
				this->Button3 = (gcnew System::Windows::Forms::Button());
				this->CheckedListBox1 = (gcnew System::Windows::Forms::CheckedListBox());
				this->Button6 = (gcnew System::Windows::Forms::Button());
				this->Label3 = (gcnew System::Windows::Forms::Label());
				this->Label2 = (gcnew System::Windows::Forms::Label());
				this->Button2 = (gcnew System::Windows::Forms::Button());
				this->Button4 = (gcnew System::Windows::Forms::Button());
				this->ComboBox2 = (gcnew System::Windows::Forms::ComboBox());
				this->PictureBox4 = (gcnew System::Windows::Forms::PictureBox());
				this->PictureBox2 = (gcnew System::Windows::Forms::PictureBox());
				this->ComboBox1 = (gcnew System::Windows::Forms::ComboBox());
				this->PictureBox3 = (gcnew System::Windows::Forms::PictureBox());
				this->PictureBox1 = (gcnew System::Windows::Forms::PictureBox());
				this->label4 = (gcnew System::Windows::Forms::Label());
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox4))->BeginInit();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox2))->BeginInit();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox3))->BeginInit();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox1))->BeginInit();
				this->SuspendLayout();
				// 
				// Button1
				// 
				this->Button1->Location = System::Drawing::Point(12, 12);
				this->Button1->Name = L"Button1";
				this->Button1->Size = System::Drawing::Size(124, 23);
				this->Button1->TabIndex = 0;
				this->Button1->Text = L"������ � ������";
				this->Button1->UseVisualStyleBackColor = true;
				this->Button1->Click += gcnew System::EventHandler(this, &Form1::Button1_Click);
				// 
				// Button11
				// 
				this->Button11->Location = System::Drawing::Point(12, 41);
				this->Button11->Name = L"Button11";
				this->Button11->Size = System::Drawing::Size(75, 23);
				this->Button11->TabIndex = 1;
				this->Button11->Text = L"50 ������";
				this->Button11->UseVisualStyleBackColor = true;
				this->Button11->Click += gcnew System::EventHandler(this, &Form1::Button11_Click);
				// 
				// Button5
				// 
				this->Button5->Location = System::Drawing::Point(12, 70);
				this->Button5->Name = L"Button5";
				this->Button5->Size = System::Drawing::Size(75, 23);
				this->Button5->TabIndex = 2;
				this->Button5->Text = L"200 ������";
				this->Button5->UseVisualStyleBackColor = true;
				this->Button5->Click += gcnew System::EventHandler(this, &Form1::Button5_Click);
				// 
				// Label1
				// 
				this->Label1->AutoSize = true;
				this->Label1->Location = System::Drawing::Point(93, 75);
				this->Label1->Name = L"Label1";
				this->Label1->Size = System::Drawing::Size(13, 13);
				this->Label1->TabIndex = 3;
				this->Label1->Text = L"0";
				// 
				// Button3
				// 
				this->Button3->Location = System::Drawing::Point(12, 99);
				this->Button3->Name = L"Button3";
				this->Button3->Size = System::Drawing::Size(75, 23);
				this->Button3->TabIndex = 4;
				this->Button3->Text = L"1 ����";
				this->Button3->UseVisualStyleBackColor = true;
				this->Button3->Click += gcnew System::EventHandler(this, &Form1::Button3_Click);
				// 
				// CheckedListBox1
				// 
				this->CheckedListBox1->FormattingEnabled = true;
				this->CheckedListBox1->Location = System::Drawing::Point(12, 222);
				this->CheckedListBox1->Name = L"CheckedListBox1";
				this->CheckedListBox1->Size = System::Drawing::Size(124, 94);
				this->CheckedListBox1->TabIndex = 5;
				this->CheckedListBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::CheckedListBox1_SelectedIndexChanged);
				this->CheckedListBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::CheckedListBox1_MouseUp);
				// 
				// Button6
				// 
				this->Button6->Location = System::Drawing::Point(12, 331);
				this->Button6->Name = L"Button6";
				this->Button6->Size = System::Drawing::Size(124, 53);
				this->Button6->TabIndex = 6;
				this->Button6->Text = L"����������� �������� �������� ����������";
				this->Button6->UseVisualStyleBackColor = true;
				this->Button6->Click += gcnew System::EventHandler(this, &Form1::Button6_Click);
				// 
				// Label3
				// 
				this->Label3->AutoSize = true;
				this->Label3->Location = System::Drawing::Point(93, 387);
				this->Label3->Name = L"Label3";
				this->Label3->Size = System::Drawing::Size(28, 13);
				this->Label3->TabIndex = 7;
				this->Label3->Text = L"���.";
				// 
				// Label2
				// 
				this->Label2->AutoSize = true;
				this->Label2->Location = System::Drawing::Point(93, 486);
				this->Label2->Name = L"Label2";
				this->Label2->Size = System::Drawing::Size(36, 13);
				this->Label2->TabIndex = 9;
				this->Label2->Text = L"����.";
				// 
				// Button2
				// 
				this->Button2->Location = System::Drawing::Point(12, 430);
				this->Button2->Name = L"Button2";
				this->Button2->Size = System::Drawing::Size(124, 53);
				this->Button2->TabIndex = 8;
				this->Button2->Text = L"����������� ����� ��������";
				this->Button2->UseVisualStyleBackColor = true;
				this->Button2->Click += gcnew System::EventHandler(this, &Form1::Button2_Click);
				// 
				// Button4
				// 
				this->Button4->Location = System::Drawing::Point(12, 582);
				this->Button4->Name = L"Button4";
				this->Button4->Size = System::Drawing::Size(74, 36);
				this->Button4->TabIndex = 10;
				this->Button4->Text = L"������� �������";
				this->Button4->UseVisualStyleBackColor = true;
				this->Button4->Click += gcnew System::EventHandler(this, &Form1::Button4_Click);
				// 
				// ComboBox2
				// 
				this->ComboBox2->FormattingEnabled = true;
				this->ComboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"��������� ��������"});
				this->ComboBox2->Location = System::Drawing::Point(142, 12);
				this->ComboBox2->Name = L"ComboBox2";
				this->ComboBox2->Size = System::Drawing::Size(250, 21);
				this->ComboBox2->TabIndex = 11;
				// 
				// PictureBox4
				// 
				this->PictureBox4->Location = System::Drawing::Point(142, 39);
				this->PictureBox4->Name = L"PictureBox4";
				this->PictureBox4->Size = System::Drawing::Size(250, 250);
				this->PictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
				this->PictureBox4->TabIndex = 12;
				this->PictureBox4->TabStop = false;
				// 
				// PictureBox2
				// 
				this->PictureBox2->Location = System::Drawing::Point(142, 322);
				this->PictureBox2->Name = L"PictureBox2";
				this->PictureBox2->Size = System::Drawing::Size(250, 250);
				this->PictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
				this->PictureBox2->TabIndex = 14;
				this->PictureBox2->TabStop = false;
				// 
				// ComboBox1
				// 
				this->ComboBox1->FormattingEnabled = true;
				this->ComboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(6) {L"���. ����������� (��������)", L"���. ������������ (��������)", 
					L"���. ���������", L"���. ��������", L"��. ������. �������� �������� (��� 0)", L"��. ������. ��������-���������� (��� 1)"});
				this->ComboBox1->Location = System::Drawing::Point(142, 295);
				this->ComboBox1->Name = L"ComboBox1";
				this->ComboBox1->Size = System::Drawing::Size(250, 21);
				this->ComboBox1->TabIndex = 13;
				// 
				// PictureBox3
				// 
				this->PictureBox3->Location = System::Drawing::Point(92, 578);
				this->PictureBox3->Name = L"PictureBox3";
				this->PictureBox3->Size = System::Drawing::Size(300, 40);
				this->PictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
				this->PictureBox3->TabIndex = 15;
				this->PictureBox3->TabStop = false;
				// 
				// PictureBox1
				// 
				this->PictureBox1->Location = System::Drawing::Point(398, 12);
				this->PictureBox1->Name = L"PictureBox1";
				this->PictureBox1->Size = System::Drawing::Size(606, 606);
				this->PictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
				this->PictureBox1->TabIndex = 16;
				this->PictureBox1->TabStop = false;
				// 
				// label4
				// 
				this->label4->Location = System::Drawing::Point(12, 151);
				this->label4->Name = L"label4";
				this->label4->Size = System::Drawing::Size(124, 68);
				this->label4->TabIndex = 17;
				this->label4->Text = L"����� ��������� ��������� ����������, ������� ����� ������������ ��� ������������" 
					L"�:";
				// 
				// Form1
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(1016, 630);
				this->Controls->Add(this->label4);
				this->Controls->Add(this->PictureBox1);
				this->Controls->Add(this->PictureBox3);
				this->Controls->Add(this->PictureBox2);
				this->Controls->Add(this->ComboBox1);
				this->Controls->Add(this->PictureBox4);
				this->Controls->Add(this->ComboBox2);
				this->Controls->Add(this->Button4);
				this->Controls->Add(this->Label2);
				this->Controls->Add(this->Button2);
				this->Controls->Add(this->Label3);
				this->Controls->Add(this->Button6);
				this->Controls->Add(this->CheckedListBox1);
				this->Controls->Add(this->Button3);
				this->Controls->Add(this->Label1);
				this->Controls->Add(this->Button5);
				this->Controls->Add(this->Button11);
				this->Controls->Add(this->Button1);
				this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
				this->MaximizeBox = false;
				this->Name = L"Form1";
				this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
				this->Text = L"�������� ����������";
				this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox4))->EndInit();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox2))->EndInit();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox3))->EndInit();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox1))->EndInit();
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
			
			
			// ���������� ����
			Brain::Core ^bcore;
			// ������� �.�.: �� ������, ���� ����� �������� �������
			VA::FormCortex2 ^formcortex2;
			
			
			// ���������� ��������� �� ������� �� ����� ����������
			void block () {
				this->Enabled = false;
				this->ComboBox1->Enabled = false;
				this->ComboBox2->Enabled = false;
				this->CheckedListBox1->Enabled = false;
				this->Button1->Enabled = false;
				this->Button2->Enabled = false;
				this->Button3->Enabled = false;
				this->Button4->Enabled = false;
				this->Button5->Enabled = false;
				this->Button6->Enabled = false;
				this->Button11->Enabled = false;
			}
			
			
			// ������������� ��������� ����� ���������� ��������
			void unblock () {
				this->Enabled = true;
				this->ComboBox1->Enabled = true;
				this->ComboBox2->Enabled = true;
				this->CheckedListBox1->Enabled = true;
				this->Button1->Enabled = true;
				this->Button2->Enabled = true;
				this->Button3->Enabled = true;
				this->Button4->Enabled = true;
				this->Button5->Enabled = true;
				this->Button6->Enabled = true;
				this->Button11->Enabled = true;
			}


			// �������������
			void Form1_Load(Object ^sender, EventArgs ^e)
			{
				System::Drawing::Point ^point = gcnew System::Drawing::Point(0, 0);
				this->Location = *point;
				
				this->bcore = gcnew Brain::Core();
				
				for (int i = 0; i < this->bcore->NP; i++) {
					CheckedListBox1->Items->Add(i);
					this->bcore->CheckedPatterns->Add(i);
					CheckedListBox1->SetItemChecked(i, true);
				}
				this->bcore->PatternsActivation();
				PictureBox1->Image = this->bcore->pic;
				PictureBox1->Update();

				PictureBox3->Image = this->bcore->pic2;
				PictureBox3->Update();

				CheckedListBox1->CheckOnClick = true;

				ComboBox1->SelectedIndex = 0;
				ComboBox2->SelectedIndex = 0;

				this->DrawSomeActivity(); // ����������� ���������� ���� ����������
				this->DrawLearningPicture(); // ����������� �������� ��������
			}
			
			
			// ������ "������ � ������"
			void Button1_Click(Object ^sender, EventArgs ^e)
			{
				// CheckedListBox1->Items->Clear();
				// PatCreate();
				
				//
				// ����������� (������� �.�.):
				//
				// ����� ���� �� ������ "������� ���" (�.�. �������� ��������� ����������),
				// � ����� �� ������ �������:
				delete this->bcore->cortex2;
				this->bcore->cortex2 = nullptr;
				delete this->bcore;
				this->bcore = nullptr;

				delete formcortex2;
				formcortex2 = nullptr;
				
				Button4->Text = "������� �������";
				Label1->Text = "0";
				Label2->Text = "����.";
				Label3->Text = "���.";

				Button2->Enabled = true;

				CheckedListBox1->Items->Clear();

				this->Form1_Load(sender, e);
			}

			
			// ����������� �������� ��������
			void DrawLearningPicture() {
				if (this->bcore->FLearn) {	
					switch (ComboBox2->SelectedIndex) {
						case 0:
							this->bcore->Draw_Learn();
							PictureBox4->Image = this->bcore->pic1;
							PictureBox4->Update();
							break;
						case 1:
							break;
						case 2:
							break;
						case 3:
							break;
						case 4:
							break;
					}
				}
				else {
					PictureBox4->Image = nullptr;
					PictureBox4->Update();
				}
			}

			
			// ������� �.�.: ���� �������� ���, ��� ������� � �������� � ������������ ����������� ������ �����
			void Tact() { 
				this->bcore->Wave();
				
				this->DrawSomeActivity(); // ����������� ���������� ���� ����������
				
				PictureBox3->Image = this->bcore->pic2;
				PictureBox3->Update();
				
				PictureBox1->Image = this->bcore->pic;
				PictureBox1->Update();
				
				if (this->bcore->cortex2 != nullptr) {
					formcortex2->PictureBox1->Image = this->bcore->cortex2->pic;
					formcortex2->PictureBox1->Update();
				}

				if (this->bcore->FPic == false && Label2->Text == "���.") {
					Label2->Text = "����.";
					Label2->Update();
					PictureBox1->Image = this->bcore->Bigpic;
					PictureBox1->Update();
					
					Button2->Enabled = true; // ������� �.�.: ����� �������� ������, ������ ����� ��������������

					this->SetTopLevel(true);
					MessageBox::Show("����� �������� ������");
				}

				this->DrawLearningPicture(); // ����������� �������� ��������
				
				Label1->Text = this->bcore->CT.ToString();
				Label1->Update();
			}

			
			
			// ������ "1 ����"
			// ����������� (������� �.�.):
			// ������ ���� ��������� ����� (������ ����� �����)
			void Button3_Click(Object ^sender, EventArgs ^e)
			{
				block();
				this->Tact();
				unblock();

				// ������� �.�.: ���������� ������ ������� �������
				this->bcore->Draw_Numbers();
				PictureBox1->Image = this->bcore->pic;
				PictureBox1->Update();
			}

			
			// 50 ������ �����
			void Button11_Click(Object ^sender, EventArgs ^e)
			{
				int NT = 50;
				block();
				for (int n = 0; n < NT; n++)
					this->Tact();
				unblock();

				// ������� �.�.: ���������� ������ ������� �������
				this->bcore->Draw_Numbers();
				PictureBox1->Image = this->bcore->pic;
				PictureBox1->Update();
			}

			
			// 200 ������ �����
			void Button5_Click(Object ^sender, EventArgs ^e)
			{
				int NT = 200;
				block();
				for (int n = 0; n < NT; n++)
					this->Tact();
				unblock();

				// ������� �.�.: ���������� ������ ������� �������
				this->bcore->Draw_Numbers();
				PictureBox1->Image = this->bcore->pic;
				PictureBox1->Update();
			}

			
			// ���. ����� ��������
			void Button2_Click(Object ^sender, EventArgs ^e)
			{
				Button2->Enabled = false; // ������� �.�.: ����������� ������ �� ����� ���������� ����� ��������
				this->bcore->FPic = ! this->bcore->FPic;

				if (this->bcore->FPic) {
					Label2->Text = "���.";
					this->bcore->PicStart = this->bcore->CT;
				}
				else {
					Label2->Text = "����.";
				}
			}


			
			// ������� �.�.: ���������� �� ������� ����
			// ����������� ���������� ���� ����������
			void DrawSomeActivity() {
				switch (ComboBox1->SelectedIndex) {
					case 0:
						this->bcore->Draw_ActAccum(); // ���. ����������� (��������)
						PictureBox2->Image = this->bcore->picActAccum;
						break;
					case 1:
						this->bcore->Draw_FieldA(); // ���. ������������ (��������)
						PictureBox2->Image = this->bcore->picFieldA;
						break;
					case 2:
						this->bcore->Draw_EvokedA(); // ���. ���������
						PictureBox2->Image = this->bcore->picEvokedA;
						break;
					case 3:
						this->bcore->Draw_WaveA(); // ���. ��������
						PictureBox2->Image = this->bcore->picWaveA;
						break;
					case 4:
						this->bcore->Draw_RelaxA(0); // ������� ���������� �������� �������� (��� 0)
						PictureBox2->Image = this->bcore->picRelaxA;
						break;
					case 5:
						this->bcore->Draw_RelaxA(1); // ������� ���������� ��������-���������� (��� 1)
						PictureBox2->Image = this->bcore->picRelaxA;
						break;
					/*case 6:
						this->bcore->MakePicAAvg(); // ������� ���� ����������, ������������ �� �������� (���� ����� �� ������������)
						PictureBox2->Image = this->bcore->picAAvg;
						break;*/
				}
				PictureBox2->Update();
			}
			
			
			// ��������/�������� ��������� �������
			void Button4_Click(Object ^sender, EventArgs ^e)
			{
				if (Button4->Text == "������� �������") {
					// �������� ����������������� ������� � ������� ���� �� ������
					this->bcore->cortex2 = gcnew Brain::Core();
					this->bcore->cortex2->cortex0 = this->bcore;
					
					// ���������� ������� ����
					this->bcore->cortex2->Draw_Status();
					formcortex2 = gcnew VA::FormCortex2();
					formcortex2->PictureBox1->Image = this->bcore->cortex2->pic;
					formcortex2->PictureBox1->Update();
					
					// ����� ������� ����
					System::Drawing::Point ^point = gcnew System::Drawing::Point(Screen::PrimaryScreen->Bounds.Width - formcortex2->Size.Width, Screen::PrimaryScreen->Bounds.Height - formcortex2->Size.Height);
					formcortex2->Location = *point;
					formcortex2->Show();

					Button4->Text = "������� �������";
				}
				else // Button4->Text == "������� �������"
				{
					delete this->bcore->cortex2;
					this->bcore->cortex2 = nullptr;

					delete formcortex2;
					formcortex2 = nullptr;
					
					Button4->Text = "������� �������";
				}

				this->bcore->Draw_Status();
				// ������� �.�.: ���������� ������ ������� �������
				this->bcore->Draw_Numbers();
				PictureBox1->Image = this->bcore->pic;
				PictureBox1->Update();
			}

			
			// ���/���� ����������� �������� ��������
			void Button6_Click(Object ^sender, EventArgs ^e)
			{
				this->bcore->FLearn = ! this->bcore->FLearn;

				if (this->bcore->FLearn) {
					Label3->Text = "���.";
				}
				else {
					Label3->Text = "����.";
				}

				this->DrawLearningPicture(); // ����������� �������� ��������
			}

		
			// ������� �.�.: ����� ��������� ��������� ���������� ��� �������������
			void CheckedListBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				this->bcore->CheckedPatterns->Clear();
				for (int i = 0; i < CheckedListBox1->CheckedItems->Count; i++) {
					this->bcore->CheckedPatterns->Add(Convert::ToInt32(CheckedListBox1->CheckedItems[i]->ToString()));
				}
			}

			
			// ����������� ��� ���������� ������ ����
			void CheckedListBox1_MouseUp(Object ^sender, MouseEventArgs ^e)
			{
				this->bcore->ActivePatternsRefresh(); // ���������� ������� �������� ������� �������
				
				PictureBox1->Image = this->bcore->pic;
				PictureBox1->Update();
			}
	};
}

