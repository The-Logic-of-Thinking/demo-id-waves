namespace IdWaves {
	public ref class FormCortex2 : public System::Windows::Forms::Form
	{
		public:
			FormCortex2(void);
			
		protected:
			~FormCortex2();
		
		public:
			System::Windows::Forms::PictureBox^  PictureBox1;

		private:
			System::ComponentModel::Container ^components;

			void InitializeComponent(void);
		
			// Инициализация
			void FormCortex2_Load(Object ^sender, System::EventArgs ^e);
	};
}