#if USE_LOVYAN == 1

#include <LovyanGFX.hpp>

class LGVX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789 _panel_instance;

    // パネルを接続するバスの種類にあったインスタンスを用意します。
    lgfx::Bus_SPI _bus_instance; // SPIバスのインスタンス
                                 // lgfx::Bus_I2C        _bus_instance;   // I2Cバスのインスタンス
    // lgfx::Bus_Parallel8  _bus_instance;   // 8ビットパラレルバスのインスタンス
    lgfx::Light_PWM _light_instance;

public:
    // コンストラクタを作成し、ここで各種設定を行います。
    // クラス名を変更した場合はコンストラクタも同じ名前を指定してください。
    LGVX(void)
    {
        {                                      // バス制御の設定を行います。
            auto cfg = _bus_instance.config(); // バス設定用の構造体を取得します。

            // SPIバスの設定
            cfg.spi_host = SPI2_HOST; // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
            // ※ ESP-IDFバージョンアップに伴い、VSPI_HOST , HSPI_HOSTの記述は非推奨になるため、エラーが出る場合は代わりにSPI2_HOST , SPI3_HOSTを使用してください。
            cfg.spi_mode = 0;                  // SPI通信モードを設定 (0 ~ 3)
            cfg.freq_write = 40000000;         // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
            cfg.freq_read = 16000000;          // 受信時のSPIクロック
            cfg.spi_3wire = true;              // 受信をMOSIピンで行う場合はtrueを設定
            cfg.use_lock = true;               // トランザクションロックを使用する場合はtrueを設定
            cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
            // ※ ESP-IDFバージョンアップに伴い、DMAチャンネルはSPI_DMA_CH_AUTO(自動設定)が推奨になりました。1ch,2chの指定は非推奨になります。
            cfg.pin_sclk = 36; // SPIのSCLKピン番号を設定
            cfg.pin_mosi = 35; // SPIのMOSIピン番号を設定
            cfg.pin_miso = 37; // SPIのMISOピン番号を設定 (-1 = disable)
            cfg.pin_dc = 21;   // SPIのD/Cピン番号を設定  (-1 = disable)
                               // SDカードと共通のSPIバスを使う場合、MISOは省略せず必ず設定してください。
                               //*/

            _bus_instance.config(cfg);              // 設定値をバスに反映します。
            _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
        }

        {                                        // 表示パネル制御の設定を行います。
            auto cfg = _panel_instance.config(); // 表示パネル設定用の構造体を取得します。

            cfg.pin_cs = 19;   // CSが接続されているピン番号   (-1 = disable)
            cfg.pin_rst = 20;  // RSTが接続されているピン番号  (-1 = disable)
            cfg.pin_busy = -1; // BUSYが接続されているピン番号 (-1 = disable)

            // ※ 以下の設定値はパネル毎に一般的な初期値が設定されていますので、不明な項目はコメントアウトして試してみてください。

            cfg.panel_width = 240;    // 実際に表示可能な幅
            cfg.panel_height = 320;   // 実際に表示可能な高さ
            cfg.offset_x = 0;         // パネルのX方向オフセット量
            cfg.offset_y = 0;         // パネルのY方向オフセット量
            cfg.offset_rotation = 1;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
            cfg.dummy_read_pixel = 8; // ピクセル読出し前のダミーリードのビット数
            cfg.dummy_read_bits = 1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
            cfg.readable = false;     // データ読出しが可能な場合 trueに設定
            cfg.invert = true;        // パネルの明暗が反転してしまう場合 trueに設定
            cfg.rgb_order = false;    // パネルの赤と青が入れ替わってしまう場合 trueに設定
            cfg.dlen_16bit = false;   // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
            cfg.bus_shared = false;   // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

            // 以下はST7735やILI9163のようにピクセル数が可変のドライバで表示がずれる場合にのみ設定してください。
            //    cfg.memory_width     =   240;  // ドライバICがサポートしている最大の幅
            //    cfg.memory_height    =   320;  // ドライバICがサポートしている最大の高さ

            _panel_instance.config(cfg);
        }

        {
            auto cfg = _light_instance.config();
            cfg.pin_bl = 11;
            cfg.invert = true;
            cfg.freq = 44100;
            cfg.pwm_channel = 7;

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }

        setPanel(&_panel_instance); // 使用するパネルをセットします。

        if (_panel_instance.config().pin_rst >= 0)
            // lgfx::pinMode(_panel_instance.config().pin_rst, lgfx::output); // <-- not working
            pinMode(_panel_instance.config().pin_rst, OUTPUT);
    }
};

#endif