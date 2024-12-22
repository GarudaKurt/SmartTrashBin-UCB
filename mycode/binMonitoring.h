class BinMonitoring {
  private:
    const int trigCan = 7;
    const int echCan = 8;
    const int trigBottle = 9;
    const int echBottle = 10;
    const int canIR = 11;
    const int bottleIR = 12;
    const int binCanBuzzer = 2;
    const int binBottleBuzzer = 3;
    long distanceCan, durationCan;
    long distanceBottle, durationBottle;

    enum class tones {
      NOTE_C4 = 262,
      NOTE_D4 = 294,
      NOTE_E4 = 330,
      NOTE_F4 = 349,
      NOTE_G4 = 392,
      NOTE_A4 = 440,
      NOTE_B4 = 494,
      NOTE_C5 = 523
    };

    tones melody[8] = {tones::NOTE_C4, tones::NOTE_D4, tones::NOTE_E4, tones::NOTE_F4,
                       tones::NOTE_G4, tones::NOTE_A4, tones::NOTE_B4, tones::NOTE_C5};
    int noteDurations[8] = {4, 4, 4, 4, 4, 4, 4, 4}; // Quarter notes

  public:
    BinMonitoring() {
      pinMode(trigCan, OUTPUT);
      pinMode(echCan, INPUT);
      pinMode(trigBottle, OUTPUT);
      pinMode(echBottle, INPUT);
      pinMode(canIR, INPUT);
      pinMode(bottleIR, INPUT);
      pinMode(binCanBuzzer, OUTPUT);
      pinMode(binBottleBuzzer, OUTPUT);
    }
    ~BinMonitoring() {
      digitalWrite(trigCan, LOW);
      digitalWrite(echCan, LOW);
      digitalWrite(trigBottle, LOW);
      digitalWrite(echBottle, LOW);
    }

    long canBinDistance() {
      digitalWrite(trigCan, LOW);
      delayMicroseconds(2);
      digitalWrite(trigCan, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigCan, LOW);

      durationCan = pulseIn(echCan, HIGH);
      distanceCan = durationCan / 58.2;
      return distanceCan;
    }

    long bottleBinDistance() {
      digitalWrite(trigBottle, LOW);
      delayMicroseconds(2);
      digitalWrite(trigBottle, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigBottle, LOW);

      durationBottle = pulseIn(echBottle, HIGH);
      distanceBottle = durationBottle / 58.2;
      return distanceBottle;
    }

    int can_IRs() {
      return digitalRead(canIR);
    }

    int bottle_IRs() {
      return digitalRead(bottleIR);
    }

    void buzzerStart(int buzzerPin) {
      for (int i = 0; i < 8; i++) {
        int noteDuration = 1000 / noteDurations[i];
        tone(buzzerPin, static_cast<int>(melody[i]), noteDuration);

        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);

        noTone(buzzerPin);
      }
    }
};
