function doGet(e) {
  // 1. Memilih Spreadsheet yang sedang aktif
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
  
  // 2. Mencatat waktu saat data masuk (Waktu Indonesia)
  var timestamp = new Date();
  
  // 3. Menangkap parameter URL yang dikirim oleh ESP32
  // Contoh URL dari ESP32: ?suhu=34.13&lux_veml=-1&lux_bh=514.17
  var suhu = e.parameter.suhu;
  var lux_veml = e.parameter.lux_veml;
  
  // 4. Memasukkan data ke baris paling bawah secara otomatis
  // Urutan Kolom: A (Waktu), B (Suhu), C (Lux VEML), D (Lux BH1750)
  sheet.appendRow([timestamp, suhu, lux_veml]);
  
  // 5. Memberikan jawaban ke ESP32 bahwa data sukses diterima
  return ContentService.createTextOutput("Sukses Mendarat di Google Sheets!");
}