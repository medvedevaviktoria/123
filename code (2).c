sharp
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Globalization;

namespace EventPlanner
{
    public partial class MainForm : Form
    {
        // Список для хранения мероприятий
        private List<Event> events = new List<Event>();

        // Конструктор формы
        public MainForm()
        {
            InitializeComponent();
            InitializeEventTypeComboBox();
        }

        // Инициализация ComboBox типами событий
        private void InitializeEventTypeComboBox()
        {
            eventTypeComboBox.Items.Add("Детский праздник");
            eventTypeComboBox.Items.Add("День Рождения");
            eventTypeComboBox.Items.Add("Корпоратив");
            eventTypeComboBox.SelectedIndex = 0; // Устанавливаем значение по умолчанию
        }


        // Обработчик нажатия кнопки "Добавить мероприятие"
        private void addEventButton_Click(object sender, EventArgs e)
        {
             // Сброс цветов полей
            ResetErrorColors();

            // Список ошибок
            List<string> errors = new List<string>();

             // Проверка на заполненность обязательных полей
            if (string.IsNullOrWhiteSpace(eventNameTextBox.Text))
            {
                errors.Add("Название мероприятия не заполнено");
                eventNameTextBox.BackColor = Color.Red;
            }

            if (string.IsNullOrWhiteSpace(eventLocationTextBox.Text))
            {
                 errors.Add("Место проведения не заполнено");
                 eventLocationTextBox.BackColor = Color.Red;
            }

             if (string.IsNullOrWhiteSpace(contactNameTextBox.Text))
            {
                 errors.Add("ФИО контактного лица не заполнено");
                  contactNameTextBox.BackColor = Color.Red;
            }
             else if (!IsValidContactName(contactNameTextBox.Text))
            {
                 errors.Add("Неверный формат ФИО.");
                  contactNameTextBox.BackColor = Color.Red;
             }


            if (string.IsNullOrWhiteSpace(contactPhoneTextBox.Text))
            {
                 errors.Add("Номер телефона не заполнен");
                 contactPhoneTextBox.BackColor = Color.Red;
            }
             else if (!IsValidPhoneNumber(contactPhoneTextBox.Text))
            {
                  errors.Add("Неверный формат номера телефона.");
                  contactPhoneTextBox.BackColor = Color.Red;

            }

            if (string.IsNullOrWhiteSpace(contactEmailTextBox.Text))
             {
                 errors.Add("Email не заполнен");
                 contactEmailTextBox.BackColor = Color.Red;
            }
            else if (!IsValidEmail(contactEmailTextBox.Text))
             {
                 errors.Add("Неверный формат электронной почты.");
                 contactEmailTextBox.BackColor = Color.Red;
             }

            // Если есть ошибки, выводим их и прерываем
             if (errors.Count > 0)
             {
                 infoLabel.Text = string.Join("\n", errors);
                  return;
             }

            // Создание нового объекта Event
            string eventType = eventTypeComboBox.SelectedItem.ToString();
            string eventName = eventNameTextBox.Text;
            DateTime eventDateTime = eventDateTimeDateTimePicker.Value;
            string eventLocation = eventLocationTextBox.Text;
            string eventDescription = eventDescriptionTextBox.Text;
            string contactName = contactNameTextBox.Text;
            string contactPhone = contactPhoneTextBox.Text;
            string contactEmail = contactEmailTextBox.Text;

            Event newEvent = new Event(eventType, eventName, eventDateTime, eventLocation,
                                     eventDescription, contactName, contactPhone, contactEmail);

            // Добавление события в список и обновление ListBox
            events.Add(newEvent);
            UpdateEventListBox();

            // Очистка полей ввода
            ClearInputFields();
            infoLabel.Text = "Регистрация мероприятия успешна!";

        }

        // Сброс цветов полей
         private void ResetErrorColors()
         {
             eventNameTextBox.BackColor = SystemColors.Window;
             eventLocationTextBox.BackColor = SystemColors.Window;
             contactNameTextBox.BackColor = SystemColors.Window;
             contactPhoneTextBox.BackColor = SystemColors.Window;
             contactEmailTextBox.BackColor = SystemColors.Window;

        }


       // Обработчик нажатия кнопки "Отменить мероприятие"
        private void cancelEventButton_Click(object sender, EventArgs e)
        {
            if (eventListBox.SelectedIndex != -1)
            {
                events.RemoveAt(eventListBox.SelectedIndex);
                UpdateEventListBox();
                infoLabel.Text = "Мероприятие отменено.";
            }
            else
            {
                infoLabel.Text = "Не выбрано мероприятие для отмены.";
            }
        }


        // Обновление списка мероприятий в ListBox
        private void UpdateEventListBox()
        {
            eventListBox.Items.Clear();
            foreach (var evnt in events)
            {
                eventListBox.Items.Add(evnt); // Вызов ToString()
            }
        }

       // Очистка полей ввода
        private void ClearInputFields()
        {
            eventNameTextBox.Clear();
            eventLocationTextBox.Clear();
            eventDescriptionTextBox.Clear();
            contactNameTextBox.Clear();
            contactPhoneTextBox.Clear();
            contactEmailTextBox.Clear();
        }

        // Поиск мероприятия по дате
         private void searchDateButton_Click(object sender, EventArgs e)
         {
            if (string.IsNullOrEmpty(searchTextBox.Text))
            {
                infoLabel.Text = "Введите дату для поиска";
                return;
            }
            if (!DateTime.TryParseExact(searchTextBox.Text, "dd.MM.yyyy", CultureInfo.InvariantCulture, DateTimeStyles.None, out DateTime searchDate))
            {
                 infoLabel.Text = "Неверный формат даты (дд.мм.гггг).";
                return;
            }
           

            var filteredEvents = events.Where(ev => ev.EventDateTime.Date == searchDate.Date).ToList();
            if (filteredEvents.Count > 0)
            {
                 eventListBox.Items.Clear();
                 foreach (var evnt in filteredEvents)
                 {
                     eventListBox.Items.Add(evnt);
                 }
                 infoLabel.Text = $"Найдено {filteredEvents.Count} мероприятий на {searchDate.ToShortDateString()}";
            } else {
                infoLabel.Text = $"На {searchDate.ToShortDateString()} мероприятий не найдено";
            }

        }

        // Поиск мероприятия по ФИО
         private void searchContactButton_Click(object sender, EventArgs e)
         {
            if (string.IsNullOrEmpty(searchTextBox.Text))
            {
                infoLabel.Text = "Введите ФИО для поиска";
                return;
            }

            string searchName = searchTextBox.Text;

            var filteredEvents = events.Where(ev => ev.ContactName.Contains(searchName, StringComparison.OrdinalIgnoreCase)).ToList();

            if (filteredEvents.Count > 0)
            {
                eventListBox.Items.Clear();
                foreach (var evnt in filteredEvents)
                {
                    eventListBox.Items.Add(evnt);
                }
                 infoLabel.Text = $"Найдено {filteredEvents.Count} мероприятий с контактным лицом: {searchName}";
            } else {
                infoLabel.Text = $"Мероприятий с контактным лицом: {searchName} не найдено";
            }
        }

          // Валидация номера телефона
         private bool IsValidPhoneNumber(string phoneNumber)
         {
            // Пример простого формата +7(999)999-9999
           return Regex.IsMatch(phoneNumber, @"^\+7\(\d{3}\)\d{3}-\d{4}$");
         }
        
         // Валидация Email
         private bool IsValidEmail(string email)
         {
              // Проверка email с использованием регулярного выражения
              return Regex.IsMatch(email, @"^[\w-\.]+@([\w-]+\.)+[\w-]{2,4}$");
         }

          // Валидация ФИО
         private bool IsValidContactName(string contactName)
        {
             return Regex.IsMatch(contactName, @"^[a-zA-Zа-яА-Я\s-]+$");
        }


        // --- Ниже невидимый код, необходимый для работы WinForms ---
        private System.Windows.Forms.ComboBox eventTypeComboBox;
        private System.Windows.Forms.TextBox eventNameTextBox;
        private System.Windows.Forms.DateTimePicker eventDateTimeDateTimePicker;
        private System.Windows.Forms.TextBox eventLocationTextBox;
        private System.Windows.Forms.TextBox eventDescriptionTextBox;
        private System.Windows.Forms.TextBox contactNameTextBox;
        private System.Windows.Forms.TextBox contactPhoneTextBox;
        private System.Windows.Forms.TextBox contactEmailTextBox;
        private System.Windows.Forms.Button addEventButton;
        private System.Windows.Forms.Button cancelEventButton;
        private System.Windows.Forms.ListBox eventListBox;
         private System.Windows.Forms.TextBox searchTextBox;
        private System.Windows.Forms.Button searchDateButton;
        private System.Windows.Forms.Button searchContactButton;
        private System.Windows.Forms.Label infoLabel;


        private void InitializeComponent()
        {
            this.eventTypeComboBox = new System.Windows.Forms.ComboBox();
            this.eventNameTextBox = new System.Windows.Forms.TextBox();
            this.eventDateTimeDateTimePicker = new System.Windows.Forms.DateTimePicker();
            this.eventLocationTextBox = new System.Windows.Forms.TextBox();
            this.eventDescriptionTextBox = new System.Windows.Forms.TextBox();
            this.contactNameTextBox = new System.Windows.Forms.TextBox();
            this.contactPhoneTextBox = new System.Windows.Forms.TextBox();
            this.contactEmailTextBox = new System.Windows.Forms.TextBox();
            this.addEventButton = new System.Windows.Forms.Button();
            this.cancelEventButton = new System.Windows.Forms.Button();
            this.eventListBox = new System.Windows.Forms.ListBox();
            this.searchTextBox = new System.Windows.Forms.TextBox();
             this.searchDateButton = new System.Windows.Forms.Button();
             this.searchContactButton = new System.Windows.Forms.Button();
             this.infoLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // eventTypeComboBox
            // 
            this.eventTypeComboBox.FormattingEnabled = true;
            this.eventTypeComboBox.Location = new System.Drawing.Point(12, 33);
            this.eventTypeComboBox.Name = "eventTypeComboBox";
            this.eventTypeComboBox.Size = new System.Drawing.Size(200, 21);
            this.eventTypeComboBox.TabIndex = 0;
            // 
            // eventNameTextBox
            // 
            this.eventNameTextBox.Location = new System.Drawing.Point(12, 73);
            this.eventNameTextBox.Name = "eventNameTextBox";
            this.eventNameTextBox.Size = new System.Drawing.Size(200, 20);
            this.eventNameTextBox.TabIndex = 1;
            // 
            // eventDateTimeDateTimePicker
            // 
            this.eventDateTimeDateTimePicker.Location = new System.Drawing.Point(12, 113);
            this.eventDateTimeDateTimePicker.Name = "eventDateTimeDateTimePicker";
            this.eventDateTimeDateTimePicker.Size = new System.Drawing.Size(200, 20);
            this.eventDateTimeDateTimePicker.TabIndex = 2;
            // 
            // eventLocationTextBox
            // 
            this.eventLocationTextBox.Location = new System.Drawing.Point(12, 153);
            this.eventLocationTextBox.Name = "eventLocationTextBox";
            this.eventLocationTextBox.Size = new System.Drawing.Size(200, 20);
            this.eventLocationTextBox.TabIndex = 3;
            // 
            // eventDescriptionTextBox
            // 
            this.eventDescriptionTextBox.Location = new System.Drawing.Point(12, 193);
            this.eventDescriptionTextBox.Multiline = true;
            this.eventDescriptionTextBox.Name = "eventDescriptionTextBox";
            this.eventDescriptionTextBox.Size = new System.Drawing.Size(200, 80);
            this.eventDescriptionTextBox.TabIndex = 4;
            // 
            // contactNameTextBox
            // 
            this.contactNameTextBox.Location = new System.Drawing.Point(12, 293);
            this.contactNameTextBox.Name = "contactNameTextBox";
            this.contactNameTextBox.Size = new System.Drawing.Size(200, 20);
            this.contactNameTextBox.TabIndex = 5;
            // 
            // contactPhoneTextBox
            // 
            this.contactPhoneTextBox.Location = new System.Drawing.Point(12, 333);
            this.contactPhoneTextBox.Name = "contactPhoneTextBox";
            this.contactPhoneTextBox.Size = new System.Drawing.Size(200, 20);
            this.contactPhoneTextBox.TabIndex = 6;
            // 
            // contactEmailTextBox
            // 
            this.contactEmailTextBox.Location = new System.Drawing.Point(12, 373);
            this.contactEmailTextBox.Name = "contactEmailTextBox";
            this.contactEmailTextBox.Size = new System.Drawing.Size(200, 20);
            this.contactEmailTextBox.TabIndex = 7;
            // 
            // addEventButton
            // 
            this.addEventButton.Location = new System.Drawing.Point(12, 413);
            this.addEventButton.Name = "addEventButton";
            this.addEventButton.Size = new System.Drawing.Size(200, 23);
            this.addEventButton.TabIndex = 8;
            this.addEventButton.Text = "Добавить мероприятие";
            this.addEventButton.UseVisualStyleBackColor = true;
            this.addEventButton.Click += new System.EventHandler(this.addEventButton_Click);
            // 
            // cancelEventButton
            // 
            this.cancelEventButton.Location = new System.Drawing.Point(12, 442);
            this.cancelEventButton.Name = "cancelEventButton";
            this.cancelEventButton.Size = new System.Drawing.Size(200, 23);
            this.cancelEventButton.TabIndex = 9;
            this.cancelEventButton.Text = "Отменить мероприятие";
            this.cancelEventButton.UseVisualStyleBackColor = true;
            this.cancelEventButton.Click += new System.EventHandler(this.cancelEventButton_Click);
             // 
            // eventListBox
            // 
            this.eventListBox.FormattingEnabled = true;
            this.eventListBox.Location = new System.Drawing.Point(248, 33);
            this.eventListBox.Name = "eventListBox";
            this.eventListBox.Size = new System.Drawing.Size(350, 303);
            this.eventListBox.TabIndex = 10;
             //
            // searchTextBox
            //
            this.searchTextBox.Location = new System.Drawing.Point(248, 373);
            this.searchTextBox.Name = "searchTextBox";
            this.searchTextBox.Size = new System.Drawing.Size(200, 20);
            this.searchTextBox.TabIndex = 11;
             //
            // searchDateButton
            //
            this.searchDateButton.Location = new System.Drawing.Point(248, 413);
            this.searchDateButton.Name = "searchDateButton";
            this.searchDateButton.Size = new System.Drawing.Size(140, 23);
            this.searchDateButton.TabIndex = 12;
            this.searchDateButton.Text = "Поиск по дате";
            this.searchDateButton.UseVisualStyleBackColor = true;
             this.searchDateButton.Click += new System.EventHandler(this.searchDateButton_Click);
            //
             // searchContactButton
            //
            this.searchContactButton.Location = new System.Drawing.Point(458, 413);
            this.searchContactButton.Name = "searchContactButton";
            this.searchContactButton.Size = new System.Drawing.Size(140, 23);
            this.searchContactButton.TabIndex = 13;
            this.searchContactButton.Text = "Поиск по ФИО";
            this.searchContactButton.UseVisualStyleBackColor = true;
             this.searchContactButton.Click += new System.EventHandler(this.searchContactButton_Click);
            //
            // infoLabel
            //
            this.infoLabel.AutoSize = true;
            this.infoLabel.Location = new System.Drawing.Point(12, 480);
            this.infoLabel.Name = "infoLabel";
            this.infoLabel.Size = new System.Drawing.Size(0, 13);
            this.infoLabel.TabIndex = 14;
            // 
            // MainForm
            // 
            this.ClientSize = new System.Drawing.Size(630, 513);
             this.Controls.Add(this.infoLabel);
            this.Controls.Add(this.searchContactButton);
            this.Controls.Add(this.searchDateButton);
             this.Controls.Add(this.searchTextBox);
            this.Controls.Add(this.eventListBox);
            this.Controls.Add(this.cancelEventButton);
            this.Controls.Add(this.addEventButton);
            this.Controls.Add(this.contactEmailTextBox);
            this.Controls.Add(this.contactPhoneTextBox);
            this.Controls.Add(this.contactNameTextBox);
            this.Controls.Add(this.eventDescriptionTextBox);
            this.Controls.Add(this.eventLocationTextBox);
            this.Controls.Add(this.eventDateTimeDateTimePicker);
            this.Controls.Add(this.eventNameTextBox);
            this.Controls.Add(this.eventTypeComboBox);
            this.Name = "MainForm";
            this.Text = "Планировщик мероприятий";
            this.ResumeLayout(false);
            this.PerformLayout();
        }

    }

    // Класс для представления мероприятия
    public class Event
    {
        public string EventType { get; set; }
        public string EventName { get; set; }
        public DateTime EventDateTime { get; set; }
        public string EventLocation { get; set; }
        public string EventDescription { get; set; }
        public string ContactName { get; set; }
        public string ContactPhone { get; set; }
        public string ContactEmail { get; set; }


        public Event(string eventType, string eventName, DateTime eventDateTime,
                     string eventLocation, string eventDescription, string contactName,
                     string contactPhone, string contactEmail)
        {
            EventType = eventType;
            EventName = eventName;
            EventDateTime = eventDateTime;
            EventLocation = eventLocation;
            EventDescription = eventDescription;
            ContactName = contactName;
            ContactPhone = contactPhone;
            ContactEmail = contactEmail;

        }
        // Переопределение метода ToString() для отображения в ListBox
        public override string ToString()
        {
            return $"{EventDateTime.ToShortDateString()} - {EventName} ({EventType})";
        }
    }
}
