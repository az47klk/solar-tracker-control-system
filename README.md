# Solar-Tracker: Güneş Paneli Güç Takip ve Kontrol Sistemi

Bu proje, hareketli güneş paneli sistemlerinin güvenli şekilde çalışmaları için, anlık güç takibi yapmak ve çevresel verileri (sıcaklık, zaman vb.) yönetmek amacıyla geliştirilmiş bir gömülü kontrol sistemidir. Proje; donanım tasarımı (PCB), düşük seviyeli sürücü yazılımı ve endüstriyel haberleşme protokollerini kapsamaktadır.

<img width="480" height="365" alt="resim" src="https://github.com/user-attachments/assets/cc40230b-8125-4d4b-9ff1-6596fc7daea5" />

## Teknik Özellikler
Sistem, endüstriyel standart gereksinimleri dikkate alınarak tasarlanmıştır:

* **Kontrol Birimi:** STM32 Mikrodenetleyici tabanlı mimari 
* **Haberleşme:** * **RS485 (Modbus):** Uzak mesafe veri iletimi ve endüstriyel cihaz entegrasyonu.
    * **I2C & UART:** Sensör ve çevre birimleri ile iletişim.
* **Donanım Bileşenleri:**
    * **MAX485:** Diferansiyel sinyal işleme ve gürültü bağışıklığı.
    * **DS1307 RTC:** Gerçek zamanlı saat takibi ile zaman bazlı hareket senaryoları.
    * **JW2 Röle Kontrolü:** Panel hareket mekanizması için güç yönetimi.
    * **128x64 OLED:** Yerel kullanıcı arayüzü ve anlık veri izleme.
* **PCB Tasarımı:** **KiCad** kullanılarak hazırlanan, sinyal bütünlüğü ve güç dağıtımı optimize edilmiş özgün devre kartı.

## Donanım Mimarisi
Proje kapsamında geliştirilen güç takip kartı şu işlevleri yerine getirmektedir:
1.  **Veri Toplama:** Sıcaklık sensörlerinden gelen verilerin işlenmesi.
2.  **Güç Yönetimi:** Röle katmanı üzerinden motor sürücülerin kontrol edilmesi.
3.  **HMI Entegrasyonu:** Sistem durumunun (gerilim, akım, zaman, sıcaklık) OLED ekran üzerinde gösterilmesi.
4.  **Haberleşme Katmanı:** Toplanan verilerin RS485 üzerinden merkezi bir birime aktarılması.

## Dosya Yapısı
* `/Hardware`: KiCad şema ve PCB tasarım dosyaları (Gerber dahil).
* `/Software`: C++ ile geliştirilen kontrol yazılımı ve sürücü kütüphaneleri.
