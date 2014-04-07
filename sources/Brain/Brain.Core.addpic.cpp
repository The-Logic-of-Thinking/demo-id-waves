#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// Формирование серии картинок
void Brain::Core::addpic() {
	int s = this->CT - this->PicStart - 1; // Егармин К.Н.: номер картинки в серии, начина с нуля и кончая (NPic - 1)
	int Y = (int)System::Math::Floor( s / (double)this->WPic ); // Егармин К.Н.: номер строки, начиная с 0
	int X = s - Y * this->WPic; // Егармин К.Н.: номер столбца, начиная с 0
	X = X * X_C; // Егармин К.Н.: координаты нижнего левого угла очередной картинки в серии
	Y = Y * Y_C; //               на полотне большой картинки

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			this->Bigpic->SetPixel(X + ix, Y + iy, this->pic->GetPixel(ix, iy));
		}
	}

	if (s == this->NPic - 1) { // Егармин К.Н.: последняя ли картинака в серии (наборе)
		this->FPic = false;  // Егармин К.Н.: отрубаем формирование серии картинок
	}
} // Формирование серии картинок. addpic()