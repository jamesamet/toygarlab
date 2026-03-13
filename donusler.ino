// ========== SAĞ TAKİPDE ÖNDE ENGEL VARSA İŞLEMLER ==========
// Ön sensör kontrolü - Sağ duvar takip modu
int onKontrolSag() {
  int onSensorDegeri = readCalibratedSensor(ONS_PIN);

  // Önde engel var mı kontrol et
  if (onSensorDegeri < On_Duvar_Mesafe) {
    // Sağda boşluk varsa sola dönme, direkt sağ takibe devam et
    if (readCalibratedSensor(SAG_PIN) > 810) {
      return 0;
    }
    // Önce hafif geriye çek
    setMotors(-180, -254);
    

    // İlk aşama: Hızlı sert dönüş - 320 değerine kadar
    while (readCalibratedSensor(ONS_PIN) < 320) {
      // Sağda boşluk varsa, sola dönmeyi bırak ve sağ takibe geç
      if (readCalibratedSensor(SAG_PIN) > 810) {
        return 0;
      }
      setMotors(-220, 220);  // Hızlı sert sola dön
     
    }

    // İkinci aşama: Yavaş hassas dönüş - 720 değerine kadar
    while (readCalibratedSensor(ONS_PIN) < 820) {
      // Sağda boşluk varsa, sola dönmeyi bırak ve sağ takibe geç
      if (readCalibratedSensor(SAG_PIN) > 810) {
        return 0;
      }
      setMotors(-180, 120);  // Yavaş sola dön
      
    } 
    // Dönüş sonrası stabilizasyon (titreşimi önle)
    setMotors(-160, -160);  // Hafif fren
    delay(9);

    return 1;  // Engel vardı, dönüş yapıldı
  }
  return 0;  // Engel yok
}

// ========== SOL TAKİPDE ÖNDE ENGEL VARSA İŞLEMLER ==========
// Ön sensör kontrolü - Sol duvar takip modu
int onKontrolSol() {
  int onSensorDegeri = readCalibratedSensor(ONS_PIN);

  // Önde engel var mı kontrol et
  if (onSensorDegeri < On_Duvar_Mesafe) {
    // Solda boşluk varsa sağa dönme, direkt sol takibe devam et
    if (readCalibratedSensor(SOL_PIN) > 810) {
      return 0;
    }
    // Önce hafif geriye çek
    setMotors(-254, -180);
   

    // İlk aşama: Hızlı sert dönüş - 320 değerine kadar
    while (readCalibratedSensor(ONS_PIN) < 320) {
      // Solda boşluk varsa, sağa dönmeyi bırak ve sol takibe geç
      if (readCalibratedSensor(SOL_PIN) > 810) {
        return 0;
      }
      setMotors(220, -220);  // Hızlı sert sağa dön
      
    }

    // İkinci aşama: Yavaş hassas dönüş - 720 değerine kadar
    while (readCalibratedSensor(ONS_PIN) < 820) {
      // Solda boşluk varsa, sağa dönmeyi bırak ve sol takibe geç
      if (readCalibratedSensor(SOL_PIN) > 810) {
        return 0;
      }
      setMotors(120, -180);  // Yavaş sağa dön
      
    } 
    // Dönüş sonrası stabilizasyon (titreşimi önle)
    setMotors(-160, -160);  // Hafif fren
    delay(9);

    return 1;  // Engel vardı, dönüş yapıldı
  }
  return 0;  // Engel yok
}
// ================================================================Sol duvar takibi hata hesabı
