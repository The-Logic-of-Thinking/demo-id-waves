using namespace System;
using namespace System::Windows::Forms;


namespace VA {
	public ref class FormCortex2 : public System::Windows::Forms::Form
	{
		public:
			FormCortex2(void)
			{
				InitializeComponent();
			}

		protected:
			~FormCortex2()
			{
				if (components)
				{
					delete components;
				}
			}
		
		
		public: System::Windows::Forms::PictureBox^  PictureBox1;

		private: System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
			void InitializeComponent(void)
			{
				this->PictureBox1 = (gcnew System::Windows::Forms::PictureBox());
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox1))->BeginInit();
				this->SuspendLayout();
				// 
				// PictureBox1
				// 
				this->PictureBox1->Location = System::Drawing::Point(12, 12);
				this->PictureBox1->Name = L"PictureBox1";
				this->PictureBox1->Size = System::Drawing::Size(606, 606);
				this->PictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
				this->PictureBox1->TabIndex = 16;
				this->PictureBox1->TabStop = false;
				// 
				// FormCortex2
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(630, 630);
				this->ControlBox = false;
				this->Controls->Add(this->PictureBox1);
				this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
				this->MaximizeBox = false;
				this->Name = L"FormCortex2";
				this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
				this->Text = L"Открытый волновой туннель";
				this->Load += gcnew System::EventHandler(this, &FormCortex2::FormCortex2_Load);
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox1))->EndInit();
				this->ResumeLayout(false);

			}
#pragma endregion

		
			// Инициализация
			void FormCortex2_Load(Object ^sender, EventArgs ^e)
			{
				
			}
	};
}