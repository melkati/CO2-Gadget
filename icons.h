// https://sourceforge.net/projects/lcd-image-converter/
// https://notisrac.github.io/FileToCArray/ Drag and drop file on "Choose file..." button. Tick box "Treat as binary". Click "Convert". Click "Save as file" and move the header file to sketch folder. Open the sketch in IDE. Include the header file containing the array.
// https://iconarchive.com/show/farm-fresh-icons-by-fatcow/temperature-5-icon.html
// https://www.flaticon.com/search?word=bluetooth&order_by=4&type=icon
// https://phosphoricons.com/


// 'wifi-icon', 16x16px
const unsigned char iconWiFi [32] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xc0, 0x03, 0x8f, 0xf1, 0x1f, 0xf8, 0xb8, 0x1d, 0xe1, 0x87, 
	0xe7, 0xe7, 0xfc, 0x3f, 0xf8, 0x1f, 0xfb, 0xdf, 0xfe, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff
};
// 'wifi-icon-med', 16x16px
const unsigned char iconWiFiMed [32] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xe1, 0x87, 
	0xe7, 0xe7, 0xfc, 0x3f, 0xf8, 0x1f, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff
};
// 'wifi-icon-low', 16x16px
const unsigned char iconWiFiLow [32] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfc, 0x3f, 0xf8, 0x1f, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff
};
// 'wifi-icon-empty', 16x16px
const unsigned char iconWiFiEmpty [32] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff
};

// // 'iconBLE', 16x16px
// const unsigned char iconBLE [32] PROGMEM = {
// 	0xff, 0xff, 0xff, 0x7f, 0xff, 0x3f, 0xff, 0xdf, 0xff, 0xef, 0xfb, 0xdf, 0xfc, 0xbf, 0xfe, 0x7f, 
// 	0xfe, 0x7f, 0xfc, 0xbf, 0xfb, 0xdf, 0xff, 0xef, 0xff, 0xdf, 0xff, 0x3f, 0xff, 0x7f, 0xff, 0xff
// };

// 'iconBLE', 16x16px
const unsigned char iconBLE[32] PROGMEM = {
    0xf8, 0xff, 0xf8, 0x7f, 0x98, 0x3f, 0x08, 0x19, 0x80, 0x19, 0xc0, 0x19, 0xe0, 0x08, 0xf0, 0x08,
	0xf0, 0x08, 0xe0, 0x08, 0xc0, 0x19, 0x80, 0x19, 0x08, 0x1b, 0x98, 0x3f, 0xf8, 0x7f, 0xf8, 0xff};

// 'iconMQTT', 16x16px
const unsigned char iconMQTT[32] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0x87, 0xe1, 0x87, 0xe1, 0x83, 0xc1, 0x83, 0xc1, 0x89, 0x91, 0x89, 0x91, 
	0x8c, 0x31, 0x8c, 0x31, 0x8e, 0x71, 0x8e, 0x71, 0x8f, 0xf1, 0x8f, 0xf1, 0xff, 0xff, 0xff, 0xff};

// 'iconEspNow', 16x16px
const unsigned char iconEspNow[32] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xc0, 0x03, 0xe6, 0x67, 0xe2, 0x47, 0xf0, 0x0f, 0xf8, 0x1f, 
	0xfc, 0x3f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff};

// 'iconUSB', 16x16px
static unsigned char iconUSB[32] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfc, 0x1f, 0xfd, 0xbf, 0x9b, 0xff, 0x03, 0x41, 
	0x04, 0x00, 0x9e, 0xff, 0xfe, 0x4f, 0xff, 0x07, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// 'iconHumidity', 16x16px
const uint16_t iconHumidity[256] PROGMEM = {
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc79f, 0xc79f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xbf7f, 0xc79f, 0xc79f, 0xf7ff, 0xcf9f, 0xc7bf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc79f, 0xc79f, 0xc79f, 0xcf9f, 0xcf9f, 0xc79f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xefdf, 0xdfbf, 0xc79f, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xffff, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xcfbf, 0xf7ff, 0xc79f, 0xffff, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xbf9f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xefdf, 0xdfbf, 0xc79f, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xbf7f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xd7bf, 0xf7ff, 0xc79f, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0xb77f, 0xb77f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xc79f, 0xe7df, 0xc79f, 0xc79f, 0x0000, 0x0000, 
0x0000, 0x0000, 0xb77f, 0x9f3f, 0x86bd, 0x86bd, 0x8ede, 0x8ede, 0x86bd, 0x86bd, 0x86bd, 0x86bd, 0xb77f, 0xc79f, 0x0000, 0x0000, 
0x0000, 0x0000, 0xb75f, 0x86bd, 0x4dfb, 0x5e5c, 0xbf5e, 0xbf5e, 0x7e9d, 0xa71e, 0x5e5c, 0x5e5c, 0xa73e, 0xc79f, 0x0000, 0x0000, 
0x0000, 0x0000, 0xb77f, 0x9f1f, 0x2d59, 0x563c, 0x6e7d, 0x86bd, 0xb73e, 0x5e5c, 0x5e5c, 0x5e5c, 0xb75f, 0xb77f, 0x0000, 0x0000, 
0x0000, 0x0000, 0xb77f, 0xa75f, 0x3d9a, 0x2d39, 0x6e3c, 0xb73e, 0x9efe, 0x9efe, 0x5e5c, 0x665c, 0xaf5f, 0xbf5f, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xaf7f, 0xa75f, 0x3d9a, 0x5dfa, 0x2d39, 0x7e7c, 0x7e5c, 0x3d9a, 0xa75f, 0xaf7f, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xaf7f, 0xa75f, 0x8ede, 0x663c, 0x5e3c, 0x86bd, 0xa75f, 0xaf7f, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xb77f, 0xaf5f, 0xaf5f, 0xaf7f, 0xaf7f, 0xb75f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

// 'iconTemperature', 16x16px
// const uint16_t iconTemperature[256] PROGMEM = {
// 0x0000, 0x0000, 0x0000, 0xfd88, 0xfd88, 0xfd88, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xfd88, 0xfd88, 0x0000, 
// 0x0000, 0x0000, 0x0000, 0xfd46, 0xfd88, 0xfd46, 0x0000, 0x0000, 0xfd4a, 0xfd68, 0xfd88, 0xfd88, 0x0000, 0xfd88, 0xfd88, 0x0000, 
// 0x0000, 0x0000, 0x0000, 0xfce2, 0xf527, 0xd402, 0x0000, 0xfdc8, 0xfd88, 0xfd88, 0xfd88, 0xfd88, 0xfd88, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0x0000, 0x0000, 0xfce2, 0xf527, 0xd402, 0x0000, 0xfd88, 0xfd88, 0xfde8, 0x0000, 0xfd88, 0xfd88, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0x0000, 0x0000, 0xfce2, 0xece6, 0xb341, 0x0000, 0xfd88, 0xfd88, 0x0000, 0x0000, 0x0000, 0xfd88, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0x0000, 0x0000, 0xfce2, 0xf527, 0xd402, 0x0000, 0xfd88, 0xfd88, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0x0000, 0x0000, 0xfce2, 0xf527, 0xd402, 0x0000, 0xfd88, 0xfd88, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0x0000, 0x0000, 0xfce2, 0xece6, 0xb341, 0x0000, 0xfd88, 0xfd88, 0x0000, 0x0000, 0xfc10, 0xfd88, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0x0000, 0x0000, 0xfce2, 0xf527, 0xd402, 0x0000, 0xfd88, 0xfd88, 0xfd88, 0xfd88, 0xfd88, 0xfd88, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0x0000, 0x0000, 0xfd05, 0xf527, 0xd423, 0x0000, 0x0000, 0xfd88, 0xfd88, 0xfd88, 0xfd88, 0xfd88, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0x0000, 0xfd88, 0xfd88, 0xfd88, 0xfdaa, 0xfd88, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0xfd04, 0xfd04, 0xfd88, 0xfd88, 0xfe50, 0xfded, 0xfd04, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0xfce3, 0xfd04, 0xfd88, 0xfd88, 0xfd88, 0xfe51, 0xfce3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0xfd03, 0xfce3, 0xfd88, 0xfd88, 0xfd88, 0xfdcc, 0xfce4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0xfd04, 0xfcc2, 0xfd04, 0xfd46, 0xfd04, 0xfcc2, 0xfd24, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
// 0x0000, 0x0000, 0xfd05, 0xfce3, 0xfcc2, 0xfce3, 0xfd45, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

// 'iconTemperature', 16x16px
const uint16_t iconTemperature[256] PROGMEM = {
0x0000, 0x0000, 0x0000, 0x0000, 0xf7df, 0xf7df, 0xe7df, 0xe7bf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xf7df, 0xef9f, 0xcefc, 0xdfbf, 0xe7bf, 0x0000, 0x6c32, 0x6c52, 0x6c52, 0x6453, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xf7df, 0xcedc, 0x8536, 0xe7bf, 0xe7bf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xf7df, 0xc6dc, 0x8536, 0xe7bf, 0xe7bf, 0x0000, 0x6c32, 0x6452, 0x6472, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xf7df, 0xc6dc, 0x8536, 0xe7bf, 0xe7bf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xf7df, 0xe452, 0xf1a9, 0xe7bf, 0xe7bf, 0x0000, 0x6473, 0x6c52, 0x6c52, 0x6453, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xf7df, 0xe3d0, 0xf967, 0xe7bf, 0xe7bf, 0x0000, 0x07ff, 0x6c32, 0x5415, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xf7df, 0xe3d0, 0xf967, 0xe7bf, 0xe7bf, 0x0000, 0x6c32, 0x6c72, 0x6452, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0xf7df, 0xe3d0, 0xf967, 0xe7bf, 0xe7bf, 0x0000, 0x8410, 0x6c32, 0x6c32, 0x07ff, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xffff, 0xf7df, 0xe3d0, 0xf967, 0xe7bf, 0xe7bf, 0x0000, 0x6c32, 0x6c72, 0x6c72, 0x6cb2, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xf7df, 0xef9f, 0xe32e, 0xf967, 0xe493, 0xdfbf, 0xe7bf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0xf7ff, 0xf7df, 0xdc11, 0xfa4a, 0xfa09, 0xf947, 0xe5b8, 0xe7bf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0xefdf, 0xf7df, 0xe30d, 0xfa4a, 0xfa4a, 0xf947, 0xe3d1, 0xdfbf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0xe7ff, 0xf7df, 0xdc93, 0xfa4a, 0xfa09, 0xf967, 0xe65a, 0xe7bf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xf7df, 0xf7df, 0xdd36, 0xe30e, 0xe5f8, 0xdfbf, 0xe7bf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xffff, 0xf7df, 0xf7df, 0xefdf, 0xe7bf, 0xe7bf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};