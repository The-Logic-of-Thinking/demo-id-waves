#include "../../headers/FormCortexes/FormCortex2.h"


IdWaves::FormCortex2::FormCortex2(void)
{
	this->InitializeComponent();
}

		
IdWaves::FormCortex2::~FormCortex2()
{
	if (components)
	{
		delete components;
	}
}
		
		
void IdWaves::FormCortex2::InitializeComponent(void)
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
	this->Text = L"ќткрытый волновой туннель";
	this->Load += gcnew System::EventHandler(this, &FormCortex2::FormCortex2_Load);
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PictureBox1))->EndInit();
	this->ResumeLayout(false);
}

	
// Инициализация
void IdWaves::FormCortex2::FormCortex2_Load(Object ^sender, System::EventArgs ^e)
{

}