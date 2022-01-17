//---------------------------------------------------------------------------

#ifndef mainformH
#define mainformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class Tmain : public TForm
{
__published:	// IDE-managed Components
        TEdit *number;
        TLabel *Label1;
        TLabel *Label2;
        TRadioButton *bigendian;
        TRadioButton *littleendian;
        TButton *okbutton;
        TEdit *location;
        void __fastcall okbuttonClick(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
        bool closerequested;
        int __fastcall generatehex(unsigned int serialnumber, unsigned int seriallocation, bool endian);
        void __fastcall strip_newline(char *text);
        unsigned char __fastcall hexfile_calculate_checksum(unsigned char *buffer, int bytes);
        unsigned char __fastcall hex_calculate_value(unsigned char high, unsigned char low);
        unsigned char __fastcall hex_char16(unsigned int value, int pos);
        unsigned char __fastcall hex_lowchar(unsigned char value);
        unsigned char __fastcall hex_highchar(unsigned char value);
        void __fastcall hexfile_generate_record(unsigned int addr, int bytes, unsigned char *inbuffer, char *outbuffer);
        void __fastcall SaveSettings(void);
        void __fastcall LoadSettings(void);
public:		// User declarations
        __fastcall Tmain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tmain *main;
//---------------------------------------------------------------------------
#endif
