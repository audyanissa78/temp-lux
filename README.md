# Sistem Monitoring Lab: Pengukur Suhu DS18B20 & Intensitas Cahaya BH1750 Berbasis ESP32 & Google Sheets 🌡️☀️

Proyek ini adalah sistem pemantauan kondisi lingkungan laboratorium (*Smart Lab*) secara otomatis dan berkala. Menggunakan mikrokontroler **ESP32**, sistem ini membaca parameter fisik berupa suhu ruangan dari sensor **DS18B20 (1-Wire)** dan intensitas cahaya dari sensor **BH1750 (Lux Meter via I2C)**. Data yang terkumpul kemudian dikirimkan secara nirkabel via koneksi Wi-Fi ke database cloud **Google Sheets** menggunakan skrip pemroses **Google Apps Script Web App**.

---

## 🚀 Fitur Utama

* **Dual-Sensor Data Acquisition:** Mengintegrasikan protokol *1-Wire* untuk mendeteksi suhu melalui sensor DS18B20 dan jalur bus *I2C* standar (GPIO 21 & 22) untuk mengukur tingkat kecerahan cahaya dalam satuan Lux melalui sensor BH1750.
* **Manajemen WiFi Interaktif Tanpa Hardcode:** Menggunakan pustaka `WiFiManager`. Jika perangkat tidak menemukan koneksi internet, ESP32 otomatis memancarkan Wi-Fi portal konfigurasi lokal agar pengguna bisa memasukkan SSID dan password secara dinamis.
* **Cloud Logging via HTTP GET:** Data suhu dan lux dikemas langsung menjadi parameter kueri URL terenkripsi HTTPS, lalu ditransmisikan secara berkala menuju server cloud Google menggunakan metode *Request HTTP GET*.
* **Serverless Backend (Apps Script GET Handler):** Mengintegrasikan fungsi penangkap data `doGet(e)` berbasis JavaScript cloud yang otomatis mengurai parameter data masuk (`suhu` dan `lux`).
* **Pencatatan Data Sekuensial Otomatis:** Memasukkan data log ke baris paling bawah Spreadsheet aktif secara *real-time* sekaligus membubuhi kolom pertama dengan objek penunjuk waktu (*timestamp*) lokal server.
* **Sistem Transmisi Terjadwal:** Pembacaan data dan pengiriman menuju cloud dikonfigurasi secara berkala dengan interval jeda aman selama 5 menit (300.000 ms) guna menghemat konsumsi daya dan bandwidth.

---

## 🔌 Konfigurasi Pin & Wiring Perangkat (ESP32)

Pastikan pin data sensor terhubung dengan benar ke pin GPIO mikrokontroler ESP32 Anda:
* **Sensor Suhu DS18B20 (1-Wire):**
  * Pin Data $\rightarrow$ GPIO 4
  * *Catatan:* Pasang resistor pull-up $4.7\text{ k}\Omega$ antara pin Data (GPIO 4) dan VCC ($3.3\text{V}$).
* **Sensor Cahaya BH1750 (Bus I2C):**
  * Pin **SDA** $\rightarrow$ GPIO 21
  * Pin **SCL** $\rightarrow$ GPIO 22

---

## 📊 Alur Kerja Data (*Data Pipeline*)

```text
 [ Sensor DS18B20 ] --(1-Wire: GPIO 4)--+
                                         |---> [ ESP32 Core ] ---> (HTTPS GET Request)
 [ Sensor BH1750  ] --(I2C: GPIO 21/22)-+          |
                                                   v
                                      [ Google Apps Script Web App ]
                                                   |
                                                   v
                                       [ Database Google Sheets ]


--- 

## 📂 Struktur Berkas Proyek

* **`sensor_suhulux.ino`** Berkas kode utama (*firmware*) ESP32 yang menangani inisialisasi sensor pembaca parameter suhu dan lux, manajemen pembuatan akses poin WiFi portal, penataan kueri tautan URL data, serta pengeksekusian metode komunikasi HTTP GET ke server.

* **`appscript.js`** Berkas kode *backend* berbasis Google Apps Script yang disebarkan sebagai Web Application. Berkas ini mendengarkan fungsi kueri data dari mikrokontroler dan mencatat hasilnya ke dalam Spreadsheet.

---

## 🛠️ Langkah Penyebaran & Instalasi (*Deployment*)

### 1. Konfigurasi Cloud (Google Sheets & Apps Script)
1. Buat dokumen **Google Sheets** baru di Google Drive Anda.
2. Buka menu **Ekstensi** $\rightarrow$ **Apps Script**.
3. Hapus semua fungsi bawaan di dalam editor, lalu salin dan tempel seluruh kode dari berkas `appscript.js`.
4. Klik tombol **Terapkan (Deploy)** $\rightarrow$ **Terapkan Baru (New Deployment)**.
5. Pada opsi jenis terapan, pilih **Aplikasi Web (Web App)**:
   * **Execute as (Jalankan sebagai):** `Me (Diri saya sendiri)`
   * **Who has access (Siapa yang memiliki akses):** `Anyone (Siapa saja)`
6. Klik **Deploy**, selesaikan proses perizinan otorisasi keamanan akun Google Anda, lalu salin bagian **ID Deployment** atau **URL Aplikasi Web** yang diterbitkan.

### 2. Pengaturan Perangkat Keras & Firmware
1. Buka file `sensor_suhulux.ino` menggunakan software Arduino IDE di komputer Anda.
2. Pastikan Anda telah menginstal pustaka berikut melalui menu *Library Manager*:
   * `BH1750`
   * `OneWire`
   * `DallasTemperature`
   * `WiFiManager`
3. Cari baris variabel `GOOGLE_SCRIPT_ID` di dalam kode, lalu ganti string di dalamnya dengan menempelkan **ID Deployment** unik yang sudah Anda salin dari langkah sebelumnya.
4. Sambungkan board ESP32 Anda ke komputer menggunakan kabel data, lalu lakukan proses *Compile* dan *Upload* program.

### 3. Aktivasi Jaringan & Pengujian Lapangan
1. Saat pertama kali ESP32 menyala, program otomatis memancarkan Wi-Fi lokal bernama **"Sensor-Lab-Setup"**.
2. Hubungkan HP atau laptop Anda ke jaringan Wi-Fi tersebut.
3. Jendela *Captive Portal* konfigurasi internet akan otomatis muncul di layar HP/laptop Anda.
4. Masukkan nama SSID dan Password Wi-Fi lokal yang memiliki akses internet di area lab Anda, lalu klik **Save**.
5. Board ESP32 Anda akan melakukan *restart* secara otomatis untuk menyambungkan diri ke internet. 
6. Buka **Serial Monitor** pada kecepatan baud `115200` untuk memantau log pembacaan suhu dan lux secara lokal, serta melihat balasan sukses *"Sukses Mendarat di Google Sheets!"* dari server Google setiap 5 menit!

--- 
