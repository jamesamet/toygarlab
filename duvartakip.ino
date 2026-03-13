
// ================== SOL DUVAR TAKİBİ HATA HESAPLAMA ==================
// Sol duvara göre robotun ne kadar sapma yaptığını hesaplar
int errorSol() {
   int solSensorDegeri = readCalibratedSensor(SOL_PIN);



  // Hata hesaplama: Robot duvara ne kadar yakın/uzak?
if (solSensorDegeri < solduvarideal + 1) {
    // Robot duvara çok yakın, sağa dönmeli (negatif hata)
    error = map(solSensorDegeri, solduvarmin, solduvarideal, -100, 0);
    kp = 0.5;  // Yumuşak düzeltme
    kd = 8;
  } else if (solSensorDegeri > solduvarideal + 11) {
    // Robot duvardan çok uzak, sola dönmeli (pozitif hata)
    error = map(solSensorDegeri, solduvarideal + 12, solduvarmax, 0, 100);
    kp = sol_u_kp;  // U dönüş için ayarlanmış değer
    kd =12;
  } else {
    // Robot ideal mesafede, hata yok
    error = 0;
  }
  return error;
}

// ================== SAĞ DUVAR TAKİBİ HATA HESAPLAMA ==================
// Sağ duvara göre robotun ne kadar sapma yaptığını hesaplar
int errorSag() {
     int sagSensorDegeri = readCalibratedSensor(SAG_PIN);


  // Hata hesaplama: Robot duvara ne kadar yakın/uzak?
 if (sagSensorDegeri < sagduvarideal + 1) {
    // Robot duvara çok yakın, sola dönmeli (pozitif hata)
    error = map(sagSensorDegeri, sagduvarmin, sagduvarideal, 100, 0);
    kp = 0.5;  // Yumuşak düzeltme
    kd = 8;
  } else if (sagSensorDegeri > sagduvarideal + 11) {
    // Robot duvardan çok uzak, sağa dönmeli (negatif hata)
    error = map(sagSensorDegeri, sagduvarideal + 12, sagduvarmax, 0, -100);
    kp = sag_u_kp;  // U dönüş için ayarlanmış değer
    kd = 12;
  } else {
    // Robot ideal mesafede, hata yok
    error = 0;
  }
  return error;
}

// ================================================================
