// Identacion de Arduino IDE (2 espacios)

#define LED_1 3
#define LED_2 4
#define BUTTON 2
#define ANTI_REBOTE 40

unsigned long tiempo_actual = 0;

void debounceFSM_init();
void debounceFSM_update();  // llamar periódicamente

void buttonPressed();
void buttonReleased();

typedef enum
{
  BUTTON_UP,
  BUTTON_FALLING,
  BUTTON_DOWN,
  BUTTON_RAISING,
}debounceState_t;

debounceState_t state;

void setup()
{
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BUTTON, INPUT);
  
  Serial.begin(9600);

  tiempo_actual = millis();
  
  debounceFSM_init();
}

void loop()
{
  debounceFSM_update();
}

void debounceFSM_init(void)
{
  if (digitalRead(BUTTON)) state = BUTTON_UP;
  else state = BUTTON_DOWN;
  if (state == BUTTON_UP) Serial.print("Init -> BUTTON_UP\n");
  else Serial.print("Init -> BUTTON_DOWN\n");
}

void debounceFSM_update()
{
  switch (state)
  {
    case BUTTON_UP:
      if (digitalRead(BUTTON) == 0)
      {
        state = BUTTON_FALLING;
        Serial.print("State -> BUTTON_FALLING\n");
      }
      break;
    case BUTTON_FALLING:
      if (millis() - tiempo_actual > ANTI_REBOTE)
      {
        if (digitalRead(BUTTON) == 0)
        {
          state = BUTTON_DOWN;
          Serial.print("Button Pressed\nState -> BUTTON_DOWN\n");
          buttonPressed();
        }
        else
        {
          state = BUTTON_UP;  
          Serial.print("Return state to BUTTON_UP\n");
        }
      }
      break;
    case BUTTON_DOWN:
      if (digitalRead(BUTTON) == 1) 
      {
        state = BUTTON_RAISING;
        Serial.print("State -> BUTTON_RAISING\n");  
      }
      break;
    case BUTTON_RAISING:
      if (millis() - tiempo_actual > ANTI_REBOTE)
      {
        if (digitalRead(BUTTON) == 1)
        {
          state = BUTTON_UP;
          Serial.print("Button Released\nState -> BUTTON_UP\n");
          buttonReleased();
        }
        else
        {
          state = BUTTON_DOWN;
          Serial.print("Returne state to BUTTON_DOWN\n");
        }  
      }
      break;
  }
}

void buttonPressed()
{
  if (digitalRead(LED_1) == 0) digitalWrite(LED_1, 1);
  else digitalWrite(LED_1, 0);
}

void buttonReleased()
{
  if (digitalRead(LED_2) == 0) digitalWrite(LED_2, 1);
  else digitalWrite(LED_2, 0);
}
