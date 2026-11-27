#ifndef ILI9341_CONSTANTS_H
#define ILI9341_CONSTANTS_H

#include <stdint.h>

/* ============================================================================
 * ILI9341 Level 1 Commands — Regular Command Set (Datasheet Section 8.2)
 * Legend: [None] = no params | [W: NB] = write N bytes | [R: NB] = read N bytes
 * ============================================================================
 */

// No Operation [None] — does nothing; terminates RAMWR/RAMRD
#define LCD_CMD_NOP ((uint8_t[]){0x00})

// Software Reset [None] — resets regs to defaults; wait >=5ms (120ms -> SLPOUT)
#define LCD_CMD_SWRESET ((uint8_t[]){0x01})

// Read Display ID Info [R: 3B + 1 dummy] — ID1 (mfr), ID2 (ver), ID3 (module)
#define LCD_CMD_RDDIDIF ((uint8_t[]){0x04})

// Read Display Status [R: 4B + 1 dummy] — BSTON, MADCTL, COLMOD, sleep/TE flags
#define LCD_CMD_RDDST ((uint8_t[]){0x09})

// Read Display Power Mode [R: 1B] — [BSTON IDMON PTLON SLPOUT NORON DISPON 0 0]
#define LCD_CMD_RDDPM ((uint8_t[]){0x0A})

// Read Display MADCTL [R: 1B] — [MY MX MV ML BGR MH 0 0]
#define LCD_CMD_RDDMADCTL ((uint8_t[]){0x0B})

// Read Pixel Format [R: 1B] — [RIM DPI[2:0] 0 DBI[2:0]] (0x55=16b, 0x66=18b)
#define LCD_CMD_RDDCOLMOD ((uint8_t[]){0x0C})

// Read Image Format [R: 1B] — [VSCROLL 0 INVON 0 0 GC[2:0]]
#define LCD_CMD_RDDIM ((uint8_t[]){0x0D})

// Read Signal Mode [R: 1B] — [TEON TEM 0 0 0 0 0 0]
#define LCD_CMD_RDDSM ((uint8_t[]){0x0E})

// Read Self-Diagnostic Result [R: 1B] — [D7=reg load OK, D6=func OK, 0..0]
#define LCD_CMD_RDDSDR ((uint8_t[]){0x0F})

// Enter Sleep Mode [None] — low-power (GRAM kept); wait >=5ms (120ms -> SLPOUT)
#define LCD_CMD_SLPIN ((uint8_t[]){0x10})

// Sleep Out [None] — wakes osc & DC/DC; wait >=5ms (120ms full wake / -> SLPIN)
#define LCD_CMD_SLPOUT ((uint8_t[]){0x11})

// Partial Mode ON [None] — displays only row window set by PTLAR (0x30)
#define LCD_CMD_PTLON ((uint8_t[]){0x12})

// Normal Display Mode ON [None] — exits Partial Mode back to full screen
#define LCD_CMD_NORON ((uint8_t[]){0x13})

// Display Inversion OFF [None] — normal pixel colors
#define LCD_CMD_INVOFF ((uint8_t[]){0x20})

// Display Inversion ON [None] — inverts displayed pixel colors
#define LCD_CMD_INVON ((uint8_t[]){0x21})

// Gamma Set [W: 1B] — GC[7:0]: curve select (0x01=G2.2; 0x02/0x04/0x08 rsvd)
#define LCD_CMD_GAMSET ((uint8_t[]){0x26})

// Display OFF [None] — blanks panel output (GRAM contents kept)
#define LCD_CMD_DISPOFF ((uint8_t[]){0x28})

// Display ON [None] — enables panel output from GRAM
#define LCD_CMD_DISPON ((uint8_t[]){0x29})

// Column Address Set [W: 4B] — SC[15:8], SC[7:0], EC[15:8], EC[7:0] (x0..x1)
#define LCD_CMD_CASET ((uint8_t[]){0x2A})

// Page Address Set [W: 4B] — SP[15:8], SP[7:0], EP[15:8], EP[7:0] (y0..y1)
#define LCD_CMD_PASET ((uint8_t[]){0x2B})

// Memory Write [W: NB] — stream pixels into CASET/PASET window (2B/px RGB565)
#define LCD_CMD_RAMWR ((uint8_t[]){0x2C})

// Color Set [W: 128B] — 16b->18b LUT: 32B Red + 64B Green + 32B Blue (6b each)
#define LCD_CMD_RGBSET ((uint8_t[]){0x2D})

// Memory Read [R: NB + 1 dummy] — read GRAM pixels (always 3B/px RGB666)
#define LCD_CMD_RAMRD ((uint8_t[]){0x2E})

// Partial Area [W: 4B] — SR[15:8], SR[7:0], ER[15:8], ER[7:0] (start/end row)
#define LCD_CMD_PTLAR ((uint8_t[]){0x30})

// Vertical Scroll Def [W: 6B] — TFA[15:0], VSA[15:0], BFA[15:0] (sum = 320)
#define LCD_CMD_VSCRDEF ((uint8_t[]){0x33})

// Tearing Effect Line OFF [None] — disables TE output pin
#define LCD_CMD_TEOFF ((uint8_t[]){0x34})

// Tearing Effect Line ON [W: 1B] — [0..0 M]: 0=V-Blank only, 1=V-Blank+H-Blank
#define LCD_CMD_TEON ((uint8_t[]){0x35})

// Memory Access Ctrl [W: 1B] — [MY MX MV ML BGR MH 0 0] (0x48=port, 0x28=land)
#define LCD_CMD_MADCTL ((uint8_t[]){0x36})

// Vertical Scroll Start Addr [W: 2B] — VSP[15:8], VSP[7:0] (GRAM line pointer)
#define LCD_CMD_VSCRSADD ((uint8_t[]){0x37})

// Idle Mode OFF [None] — full color depth (up to 262K colors)
#define LCD_CMD_IDMOFF ((uint8_t[]){0x38})

// Idle Mode ON [None] — reduced 8-color low-power mode (MSB of R/G/B only)
#define LCD_CMD_IDMON ((uint8_t[]){0x39})

// Pixel Format Set [W: 1B] — [0 DPI[2:0] 0 DBI[2:0]] (0x55=RGB565, 0x66=18b)
#define LCD_CMD_COLMOD ((uint8_t[]){0x3A})

// Write Memory Continue [W: NB] — continue streaming pixels from last GRAM addr
#define LCD_CMD_RAMWRC ((uint8_t[]){0x3C})

// Read Memory Continue [R: NB + 1 dummy] — continue reading from last GRAM addr
#define LCD_CMD_RAMRDC ((uint8_t[]){0x3E})

// Set Tear Scanline [W: 2B] — STS[8] (bit 0), STS[7:0] (scanline 0..319)
#define LCD_CMD_STE ((uint8_t[]){0x44})

// Get Scanline [R: 2B + 1 dummy] — GTS[9:8] (bits 1:0), GTS[7:0] (current line)
#define LCD_CMD_GSL ((uint8_t[]){0x45})

// Write Display Brightness [W: 1B] — DBV[7:0] (0x00=min .. 0xFF=max PWM)
#define LCD_CMD_WRDISBV ((uint8_t[]){0x51})

// Read Display Brightness [R: 1B] — DBV[7:0]
#define LCD_CMD_RDDISBV ((uint8_t[]){0x52})

// Write CTRL Display [W: 1B] — [0 0 BCTRL 0 DD BL 0 0] (ctrl, dimming, BL on)
#define LCD_CMD_WRCTRLD ((uint8_t[]){0x53})

// Read CTRL Display [R: 1B] — [0 0 BCTRL 0 DD BL 0 0]
#define LCD_CMD_RDCTRLD ((uint8_t[]){0x54})

// Write CABC [W: 1B] — [0..0 C[1:0]]: 00=Off, 01=UI, 10=Still, 11=Moving image
#define LCD_CMD_WRCABC ((uint8_t[]){0x55})

// Read CABC [R: 1B] — [0..0 C[1:0]]
#define LCD_CMD_RDCABC ((uint8_t[]){0x56})

// Write CABC Min Brightness [W: 1B] — CMB[7:0] (0x00..0xFF min backlight floor)
#define LCD_CMD_WRCABCMB ((uint8_t[]){0x5E})

// Read CABC Min Brightness [R: 1B] — CMB[7:0]
#define LCD_CMD_RDCABCMB ((uint8_t[]){0x5F})

// Read ID1 [R: 1B] — LCD module manufacturer ID
#define LCD_CMD_RDID1 ((uint8_t[]){0xDA})

// Read ID2 [R: 1B] — LCD module/driver version ID (0x80..0xFF)
#define LCD_CMD_RDID2 ((uint8_t[]){0xDB})

// Read ID3 [R: 1B] — LCD module/driver ID
#define LCD_CMD_RDID3 ((uint8_t[]){0xDC})

/* ============================================================================
 * ILI9341 Level 2 Commands — Extended Command Set (Datasheet Section 8.3)
 * ============================================================================
 */

// RGB Interface Signal Ctrl [W: 1B] — [ByPass RCM[1:0] 0 VSPL HSPL DPL EPL]
#define LCD_CMD_IFMODE ((uint8_t[]){0xB0})

// Frame Rate Ctrl (Normal) [W: 2B] — DIVA[1:0], RTNA[4:0] (0x00,0x1B=70Hz)
#define LCD_CMD_FRMCTR1 ((uint8_t[]){0xB1})

// Frame Rate Ctrl (Idle/8-color) [W: 2B] — DIVB[1:0] (div), RTNB[4:0] (clks/ln)
#define LCD_CMD_FRMCTR2 ((uint8_t[]){0xB2})

// Frame Rate Ctrl (Partial) [W: 2B] — DIVC[1:0] (div), RTNC[4:0] (clks/line)
#define LCD_CMD_FRMCTR3 ((uint8_t[]){0xB3})

// Display Inversion Ctrl [W: 1B] — [0..0 NLA NLB NLC] (0=line, 1=frame inv)
#define LCD_CMD_INVTR ((uint8_t[]){0xB4})

// Blanking Porch Ctrl [W: 4B] — VFP[6:0], VBP[6:0], HFP[4:0], HBP[4:0]
#define LCD_CMD_PRCTR ((uint8_t[]){0xB5})

// Display Function Ctrl [W: 4B] — [PTG PT], [REV GS SS SM ISC], [NL], [PCDIV]
#define LCD_CMD_DISCTRL ((uint8_t[]){0xB6})

// Entry Mode Set [W: 1B] — [0..0 GON DTE 0 0 GAS] (low-V detect / gate out)
#define LCD_CMD_ETMOD ((uint8_t[]){0xB7})

// Backlight Ctrl 1 [W: 1B] — [0..0 TH_UI[3:0]] (CABC grayscale thresh, UI)
#define LCD_CMD_BLCTRL1 ((uint8_t[]){0xB8})

// Backlight Ctrl 2 [W: 1B] — [TH_MV[3:0] TH_ST[3:0]] (CABC thresh, Mv/Still)
#define LCD_CMD_BLCTRL2 ((uint8_t[]){0xB9})

// Backlight Ctrl 3 [W: 1B] — [0..0 DTH_UI[3:0]] (CABC min thresh, UI mode)
#define LCD_CMD_BLCTRL3 ((uint8_t[]){0xBA})

// Backlight Ctrl 4 [W: 1B] — [DTH_MV[3:0] DTH_ST[3:0]] (CABC min, Moving/Still)
#define LCD_CMD_BLCTRL4 ((uint8_t[]){0xBB})

// Backlight Ctrl 5 [W: 1B] — [DIM2[3:0] 0 DIM1[2:0]] (dimming step & frames)
#define LCD_CMD_BLCTRL5 ((uint8_t[]){0xBC})

// Backlight Ctrl 7 [W: 1B] — PWM_DIV[7:0] (LEDPWM output frequency divider)
#define LCD_CMD_BLCTRL7 ((uint8_t[]){0xBE})

// Backlight Ctrl 8 [W: 1B] — [0..0 LEDONR LEDONPOL LEDPWMPOL] (LED pin ctrl)
#define LCD_CMD_BLCTRL8 ((uint8_t[]){0xBF})

// Power Ctrl 1 [W: 1B] — [0 0 VRH[5:0]] (GVDD: 0x03=3.0V..0x3D=5.9V, 0x23=4.6V)
#define LCD_CMD_PWCTRL1 ((uint8_t[]){0xC0})

// Power Ctrl 2 [W: 1B] — [0..0 BT[2:0]] (step-up circuit factor for VGH/VGL)
#define LCD_CMD_PWCTRL2 ((uint8_t[]){0xC1})

// VCOM Ctrl 1 [W: 2B] — [0 VMH[6:0]] (2.7..5.875V), [0 VML[6:0]] (-2.5..0V)
#define LCD_CMD_VMCTRL1 ((uint8_t[]){0xC5})

// VCOM Ctrl 2 [W: 1B] — [nVM VMF[6:0]] (set nVM=1 to use VMF offset vs MTP)
#define LCD_CMD_VMCTRL2 ((uint8_t[]){0xC7})

// NV Memory Write [W: 2B] — PGM_ADR[2:0] (ID1..3/VCOM), PGM_DATA[7:0] (MTP)
#define LCD_CMD_NVMWR ((uint8_t[]){0xD0})

// NV Memory Protection Key [W: 3B] — write 0x55, 0xAA, 0x66 to unlock MTP
#define LCD_CMD_NVMPKEY ((uint8_t[]){0xD1})

// NV Memory Status Read [R: 2B + 1 dummy] — [ID2_CNT ID1_CNT], [BUSY VMF ID3]
#define LCD_CMD_RDNVM ((uint8_t[]){0xD2})

// Read ID4 [R: 3B + 1 dummy] — returns 0x00, 0x93, 0x41 (IC model = ILI9341)
#define LCD_CMD_RDID4 ((uint8_t[]){0xD3})

// Positive Gamma Correction [W: 15B] — 15 grayscale tuning bytes (VP63..VP0)
#define LCD_CMD_PGAMCTRL ((uint8_t[]){0xE0})

// Negative Gamma Correction [W: 15B] — 15 grayscale tuning bytes (VN63..VN0)
#define LCD_CMD_NGAMCTRL ((uint8_t[]){0xE1})

// Digital Gamma Ctrl 1 [W: 16B] — 16B macro-adj [RCAx[3:0] BCAx[3:0]] (0..15)
#define LCD_CMD_DGAMCTRL1 ((uint8_t[]){0xE2})

// Digital Gamma Ctrl 2 [W: 64B] — 64B micro-adj [RFAx[3:0] BFAx[3:0]] (0..63)
#define LCD_CMD_DGAMCTRL2 ((uint8_t[]){0xE3})

// Interface Ctrl [W: 3B] — [MY/MX/MV/BGR_EOR WEMODE], [EPF MDT], [END DM RM]
#define LCD_CMD_IFCTL ((uint8_t[]){0xF6})

/* ============================================================================
 * ILI9341 Extended Register Commands (Datasheet Section 8.4 — requires EXTC=1)
 * ============================================================================
 */

// Power Ctrl A [W: 5B] — 0x39, 0x2C, 0x00, REG_VD[2:0] (Vcore), VBC[2:0]
#define LCD_CMD_PWCTRLA ((uint8_t[]){0xCB})

// Power Ctrl B [W: 3B] — 0x00, [1 0 DRV_vml PC 1], [DC_ena ..] (pwr save / ESD)
#define LCD_CMD_PWCTRLB ((uint8_t[]){0xCF})

// Driver Timing Ctrl A (Int Clk) [W: 3B] — [0x84|NOW], [EQ|CR], [0x78|PC[1:0]]
#define LCD_CMD_DTCTRLA_INT ((uint8_t[]){0xE8})

// Driver Timing Ctrl A (Ext Clk) [W: 3B] — [0x84|NOWE], [EQE|CRE], [0x78|PCE]
#define LCD_CMD_DTCTRLA_EXT ((uint8_t[]){0xE9})

// Driver Timing Ctrl B [W: 2B] — [VG_SW_T4 T3 T2 T1] (gate timing), 0x00
#define LCD_CMD_DTCTRLB ((uint8_t[]){0xEA})

// Power On Seq Ctrl [W: 4B] — [CP1/23 soft], [En_vcl/ddvdh], [vgh/vgl], [enh]
#define LCD_CMD_PWONSEQ ((uint8_t[]){0xED})

// Enable 3G [W: 1B] — [0..0 1 3G_enb] (0x00=disable 3-gamma, 0x02=enable)
#define LCD_CMD_EN3G ((uint8_t[]){0xF2})

// Pump Ratio Ctrl [W: 1B] — [0 0 Ratio[1:0] 0..0] (0x20=2xVCI, 0x30=3xVCI)
#define LCD_CMD_PUMPRATIO ((uint8_t[]){0xF7})

#endif // ILI9341_CONSTANTS_H
