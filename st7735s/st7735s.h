enum st7789v_command {
    NOP       = 0x00,   // No operation
    SWRESET   = 0x01,   // Software reset
    RDDID     = 0x04,   // Read display identification information
    RDDST     = 0x09,   // Read display status
    RDDPM     = 0x0A,   // Read display power mode
    RDDMADCTL = 0x0B,   // Read display MADCTL
    RDDCOLMOD = 0x0C,   // Read display pixel format
    RDDIM     = 0x0D,   // Read display image mode
    RDDSM     = 0x0E,   // Read display signal mode
    RDDSDR    = 0x0F,   // Read display self-diagnostic result
    SLPIN     = 0x10,   // Enter sleep mode
    SLPOUT    = 0x11,   // Sleep out
    PTLON     = 0x12,   // Partial mode on
    NORON     = 0x13,   // Normal display mode on
    INVOFF    = 0x20,   // Display inversion off
    INVON     = 0x21,   // Display inversion on
    GAMSET    = 0x26,   // Gamma set
    DISPOFF   = 0x28,   // Display off
    DISPON    = 0x29,   // Display on
    CASET     = 0x2A,   // Column address set
    RASET     = 0x2B,   // Row address set
    RAMWR     = 0x2C,   // Memory write
    RAMRD     = 0x2E,   // Memory read
    PTLAR     = 0x30,   // Partial start/end address set
    // VSCRDEF   = 0x33,   // Vertical scrolling definition
    TEOFF     = 0x34,   // Tearing effect line off
    TEON      = 0x35,   // Tearing effect line on
    MADCTL    = 0x36,   // Memory data access control
    // VSCRSADD  = 0x37,   // Vertical scrolling start address
    IDMOFF    = 0x38,   // Idle mode off
    IDMON     = 0x39,   // Idle mode on
    COLMOD    = 0x3A,   // Interface pixel format
    // RAMWRC    = 0x3C,   // Memory write continue
    // RAMRDC    = 0x3E,   // Memory read continue
    // TESCAN    = 0x44,   // Set tear scanline
    // RDTESCAN  = 0x45,   // Read tear scanline
    // WRDISBV   = 0x51,   // Write display brightness
    // RDDISBV   = 0x52,   // Read display brightness
    // WRCTRLD   = 0x53,   // Write control display
    // RDCTRLD   = 0x54,   // Read control display
    // WRCACE    = 0x55,   // Write content adaptive brightness control
    // RDCACB    = 0x56,   // Read content adaptive brightness control
    // WRCABCMB  = 0x5E,   // Write CABC minimum brightness
    // RDCABCMB  = 0x5F,   // Read CABC minimum brightness
    // RDABCSDR  = 0x68,   // Read auto brightness control self-diagnostic result
    RDID1     = 0xDA,   // Read ID1
    RDID2     = 0xDB,   // Read ID2
    RDID3     = 0xDC,   // Read ID3
};