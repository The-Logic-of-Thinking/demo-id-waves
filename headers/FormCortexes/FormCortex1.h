#include "../Brain/Brain.h"
#include "FormCortex2.h"


namespace IdWaves {
	public ref class FormCortex1 : public System::Windows::Forms::Form
	{
		public:
			FormCortex1(void);
			
		protected:
			~FormCortex1();
					
		private:
			System::Windows::Forms::Button^  Button1;
			System::Windows::Forms::Button^  Button11;
			System::Windows::Forms::Button^  Button5;
			System::Windows::Forms::Label^  Label1;
			System::Windows::Forms::Button^  Button3;
			System::Windows::Forms::CheckedListBox^  CheckedListBox1;
			System::Windows::Forms::Button^  Button6;
			System::Windows::Forms::Label^  Label3;
			System::Windows::Forms::Label^  Label2;
			System::Windows::Forms::Button^  Button2;
			System::Windows::Forms::Button^  Button4;
			System::Windows::Forms::ComboBox^  ComboBox2;
			System::Windows::Forms::PictureBox^  PictureBox4;
			System::Windows::Forms::PictureBox^  PictureBox2;
			System::Windows::Forms::ComboBox^  ComboBox1;
			System::Windows::Forms::PictureBox^  PictureBox3;
			System::Windows::Forms::PictureBox^  PictureBox1;
			System::Windows::Forms::Label^  label4;
			System::ComponentModel::Container ^components;

			void InitializeComponent(void);
			
			// Объявление коры
			Brain::Core ^bcore;

			// Егармин К.Н.: на случай, если будет волновой туннель
			IdWaves::FormCortex2 ^formcortex2;
			
			// Блокировка контролов от нажатий во время вычислений
			void block ();
			
			// Разблокировка контролов после завершения расчетов
			void unblock ();

			// Инициализация
			void FormCortex1_Load(Object ^sender, System::EventArgs ^e);
			
			// кнопка "Начать с начала"
			void Button1_Click(Object ^sender, System::EventArgs ^e);
			
			// Отображение картинки обучения
			void DrawLearningPicture();
			
			// Егармин К.Н.: сюда вынесено все, что связано с расчетом и отображением результатов одного такта
			void Tact();
			
			// кнопка "1 такт"
			// Комментарии (Егармин К.Н.):
			// Видимо пуск одиночной волны (одного такта волны)
			void Button3_Click(Object ^sender, System::EventArgs ^e);
			
			// 50 тактов волны
			void Button11_Click(Object ^sender, System::EventArgs ^e);
			
			// 200 тактов волны
			void Button5_Click(Object ^sender, System::EventArgs ^e);
			
			// вкл. серии картинок
			void Button2_Click(Object ^sender, System::EventArgs ^e);
			
			// Егармин К.Н.: перенесено из объекта коры
			// Отображение выбранного поля активности
			void DrawSomeActivity();
			
			// Создание/удаление волнового туннеля
			void Button4_Click(Object ^sender, System::EventArgs ^e);
			
			// Вкл/выкл отображение картинки обучения
			void Button6_Click(Object ^sender, System::EventArgs ^e);
		
			// Егармин К.Н.: Выбор паттернов вызванной активности для моделирования
			void CheckedListBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);

			// Срабатывает при отпускании кнопки мыши
			void CheckedListBox1_MouseUp(Object ^sender, System::Windows::Forms::MouseEventArgs ^e);
	};
}