#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// ������� ������ ��������� ���� (������ ��������)
void Brain::Core::Draw_Status() {
	System::Drawing::Pen ^Pen;
	int i1;
	int i2;
	
	for (int i = 0; i < this->X_C; i++) {
		for (int j = 0; j < this->Y_C; j++) {
			switch (this->Cortex[i, j].Status) {
				case 0: // ����� // 0 - ������� // ����������� (������� �.�.): ������
					this->pic->SetPixel(i, j, System::Drawing::Color::Black);
					break;
				case 1: // ��������� ���������� // 1 - ��������� ���������� // ����������� (������� �.�.): �������
					this->pic->SetPixel(i, j, System::Drawing::Color::Orange);
					break;
				case 2: // ������ ����� �������������� // 2 - �������� ���������� // ����������� (������� �.�.): �����
					this->pic->SetPixel(i, j, System::Drawing::Color::Gray);
					if (this->Cortex[i, j].ERR) {
						this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					}
					break;
				case 3: // ����� ����� �������������� // 3 - ������ ���� �������� ����������
					this->pic->SetPixel(i, j, System::Drawing::Color::White);
					if (this->Cortex[i, j].ERR) {
						this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					}
					break;
				case 4: // ������� � ������������� ��������� // 4 - ���������� ����� ��������� ������ // ����������� (������� �.�.): �����
					this->pic->SetPixel(i, j, System::Drawing::Color::DarkBlue);
					break;
				case 5: // ������� ���������� ����� // 5 - ���������� ���������� // ����������� (������� �.�.): �������
					this->pic->SetPixel(i, j, System::Drawing::Color::Green);
					break;
				case 6: // ������������ ���������� // 6 - ������� �����
					this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					break;
			}
		}
	}

	// ���� ���� �������� �������, �� ��������� ��� ������
	// if ((this->Text == "Form1" && cortex2 != nullptr) || this->Text == "2")
	// ������� �.�.: ����� ����� ���:
	if ( (this->cortex2 != nullptr) != (this->cortex0 != nullptr) ) {
		Pen = gcnew System::Drawing::Pen(System::Drawing::Color::White, 1);
		
		if ( this->cortex2 != nullptr ) {
			i1 = this->TX1;
			i2 = this->TY1;
		}
		else
		{
			i1 = this->TX2;
			i2 = this->TY2;
		}
		
		this->Gpic->DrawLine(Pen, i1, i2, i1 + TD, i2);
		this->Gpic->DrawLine(Pen, i1, i2 + TD, i1 + TD, i2 + TD);
		this->Gpic->DrawLine(Pen, i1, i2, i1, i2 + TD);
		this->Gpic->DrawLine(Pen, i1 + TD, i2, i1 + TD, i2 + TD);
	}
} // ������� ������ ��������� ���� (������ ��������). Draw_Status()