//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("ManualSerialNumber.res");
USEFORM("mainform.cpp", main);

int returnvalue = 1;

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(Tmain), &main);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return returnvalue;
}
//---------------------------------------------------------------------------
