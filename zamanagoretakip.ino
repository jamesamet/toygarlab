int zamantakipyon() {
unsigned long simdikiZaman = millis();
unsigned long gecenSure = simdikiZaman - sonDegisimZamani;

// 1. geçiş: 5 sn sonra
if (!birinciDegisimYapildi && gecenSure >= birinciGecisSuresi) {
  if (takip == 1) {
    int solDeger = readCalibratedSensor(SOL_PIN);
    if (solDeger <700) {
      takip = 0;
      birinciDegisimYapildi = true;
    }
  } else {
    int sagDeger = readCalibratedSensor(SAG_PIN);
    if (sagDeger < 700) {
      takip = 1;
      birinciDegisimYapildi = true;
    }
  }
}

// 2. geçiş: 10 sn daha sonra (toplam 15 sn)
if (birinciDegisimYapildi && !ikinciDegisimYapildi && gecenSure >= ikinciGecisSuresi) {
  if (takip == 1) {
    int solDeger = readCalibratedSensor(SOL_PIN);
    if (solDeger < 700) {
      takip = 0;
      ikinciDegisimYapildi = true;
    }
  } else {
    int sagDeger = readCalibratedSensor(SAG_PIN);
    if (sagDeger < 700) {
      takip = 1;
      ikinciDegisimYapildi = true;
    }
  }
}
}
