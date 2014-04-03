#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// ������������ ����� ��������
void Brain::Core::addpic() {
	int s = this->CT - this->PicStart - 1; // ������� �.�.: ����� �������� � �����, ������ � ���� � ������ (NPic - 1)
	int Y = (int)System::Math::Floor( s / (double)this->WPic ); // ������� �.�.: ����� ������, ������� � 0
	int X = s - Y * this->WPic; // ������� �.�.: ����� �������, ������� � 0
	X = X * X_C; // ������� �.�.: ���������� ������� ������ ���� ��������� �������� � �����
	Y = Y * Y_C; //               �� ������� ������� ��������

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			this->Bigpic->SetPixel(X + ix, Y + iy, this->pic->GetPixel(ix, iy));
		}
	}

	if (s == this->NPic - 1) { // ������� �.�.: ��������� �� ��������� � ����� (������)
		this->FPic = false;  // ������� �.�.: �������� ������������ ����� ��������
	}
} // ������������ ����� ��������. addpic()