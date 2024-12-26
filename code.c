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

        public MainForm()
        {
            InitializeComponent();
            InitializeEventTypeComboBox();
        }

         // Инициализируем ComboBox типами событий
        private void InitializeEventTypeComboBox()
        {
            eventTypeComboBox.DataSource = Enum.GetValues(typeof(EventType));
            eventTypeComboBox.SelectedIndex = 0; // Устанавливаем значение по умолчанию
        }

        private void addEventButton_Click(object sender, EventArgs e)
        {
            // Проверяем, заполнены ли обязательные поля
            if (string.IsNullOrWhiteSpace(eventNameTextBox.Text) ||
                string.IsNullOrWhiteSpace(eventLocationTextBox.Text) ||
                string.IsNullOrWhiteSpace(contactNameTextBox.Text) ||
                string.IsNullOrWhiteSpace(contactPhoneTextBox.Text) ||
                string.IsNullOrWhiteSpace(contactEmailTextBox.Text))
            {
                infoLabel.Text = "Пожалуйста, заполните все обязательные поля.";
                return;
            }


           if (!IsValidPhoneNumber(contactPhoneTextBox.Text))
            {
                infoLabel.Text = "Неверный формат номера телефона.";
                return;
            }

             if (!IsValidEmail(contactEmailTextBox.Text))
             {
                 infoLabel.Text = "Неверный формат электронной почты.";
                 return;
             }

            // Создаем новый объект Event
            EventType eventType = (EventType)eventTypeComboBox.SelectedItem;
            string eventName = eventNameTextBox.Text;
            DateTime eventDateTime = eventDateTimeDateTimePicker.Value;
            string eventLocation = eventLocationTextBox.Text;
            string eventDescription = eventDescriptionTextBox.Text;
            string contactName = contactNameTextBox.Text;
            string contactPhone = contactPhoneTextBox.Text;
            string contactEmail = contactEmailTextBox.Text;

            Event newEvent = new Event(eventType, eventName, eventDateTime, eventLocation,
                                     eventDescription, contactName, contactPhone, contactEmail);


            // Добавляем событие в список и обновляем ListBox
            events.Add(newEvent);
            UpdateEventListBox();

            // Очищаем поля формы
            ClearInputFields();
            infoLabel.Text = "Мероприятие добавлено.";
        }

       
        private void cancelEventButton_Click(object sender, EventArgs e)
        {
            if (eventListBox.SelectedIndex != -1)
            {
                events.RemoveAt(eventListBox.SelectedIndex);
                UpdateEventListBox();
                infoLabel.Text = "Мероприятие отменено.";
            } else {
                 infoLabel.Text = "Не выбрано мероприятие для отмены.";
            }
        }


        // Обновление ListBox
        private void UpdateEventListBox()
        {
            eventListBox.Items.Clear();
            foreach (var evnt in events)
            {
                eventListBox.Items.Add(evnt); // Вызов ToString()
            }
        }

       //Очищаем форму
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

        //Поиск мероприятия по ФИО
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

         private bool IsValidPhoneNumber(string phoneNumber)
         {
            // Пример простого формата +7(999)999-9999
           return Regex.IsMatch(phoneNumber, @"^\+7\(\d{3}\)\d{3}-\d{4}$");
         }
        
         private bool IsValidEmail(string email)
         {
              // Проверка email с использованием регулярного выражения
              return Regex.IsMatch(email, @"^[\w-\.]+@([\w-]+\.)+[\w-]{2,4}$");
         }
    }

    // Перечисление для типа мероприятия
    public enum EventType
    {
        Детский_праздник,
        День_Рождения,
        Корпоратив
    }

    // Класс для представления мероприятия
    public class Event
    {
        public EventType EventType { get; set; }
        public string EventName { get; set; }
        public DateTime EventDateTime { get; set; }
        public string EventLocation { get; set; }
        public string EventDescription { get; set; }
        public string ContactName { get; set; }
        public string ContactPhone { get; set; }
        public string ContactEmail { get; set; }
      

        public Event(EventType eventType, string eventName, DateTime eventDateTime,
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

