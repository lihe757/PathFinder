//#define WIN32_LEAN_AND_MEAN

#include <windows.h>   
#include <stdlib.h>
#include <time.h>

#include "CgaBob.h"
#include "defines.h"
#include "resource.h"



using namespace std;



///////////////////////GLOBALS ////////////////////////////////////

char*			szApplicationName = "Chapter3 - Pathfinder";
char*			szWindowClassName = "PFclass";

float g_fCrossoverRate =(float) CROSSOVER_RATE;
float g_fMutateRate =(float) MUTATION_RATE;
int	  g_iPopSize = POP_SIZE;
int   g_iChromoLength = CHROMO_LENGTH;
int   g_iMaxGeneration = GENERATIONS;
int   g_iShowOriginalLine=0;
int   g_iShowFixedLine =1;
//pointer to the GA object
CgaBob*	g_pGABob;
//this holds the hinstance
static HINSTANCE hInstance;
//global handle to our programs window
HWND g_hwnd = NULL;

//--------------------------------- AboutDialogProc ----------------------
//
//  message handler for the 'About' dialog box
//------------------------------------------------------------------------
BOOL CALLBACK OptionDialogProc(HWND   hwnd,
                              UINT   msg,
                              WPARAM wParam,
                              LPARAM lParam)
{

	  //get handles to edit controls
  HWND hwndCrossoverRate = GetDlgItem(hwnd, IDC_EDIT_COROSS_RATE);
  HWND hwndMutationRate= GetDlgItem(hwnd, IDC_EDIT_MUTE_RATE);
  HWND hwndPopSize= GetDlgItem(hwnd, IDC_EDIT_POP_SIZE);
  HWND hwndChromoLength= GetDlgItem(hwnd, IDC_CHORME_LENGTH);
  HWND hwndMaxGeneration= GetDlgItem(hwnd, IDC_EDIT_GENERATION);

  HWND hwndShowOrigin= GetDlgItem(hwnd, IDC_EDIT1);
  HWND hwndShowFixed= GetDlgItem(hwnd, IDC_EDIT2);
  switch(msg)
  {
  case WM_INITDIALOG:
    {
		string s = ftos(g_fCrossoverRate);
		SetWindowText(hwndCrossoverRate, s.c_str());

		s = ftos(g_fMutateRate);
		SetWindowText(hwndMutationRate, s.c_str());

		 s = itos(g_iPopSize);
		SetWindowText(hwndPopSize, s.c_str());

		s = itos(g_iChromoLength);
		SetWindowText(hwndChromoLength, s.c_str());

		s = itos(g_iMaxGeneration);
		SetWindowText(hwndMaxGeneration, s.c_str());
		//
		s = itos(g_iShowOriginalLine);
		SetWindowText(hwndShowOrigin, s.c_str());

		s = itos(g_iShowFixedLine);
		SetWindowText(hwndShowFixed, s.c_str());


      return true;
    }

    break;

  case WM_COMMAND:
    {
      switch(LOWORD(wParam))
      {
      case IDOK:
        {
			char  buffer[10];

			//-----------first the number of balls
			GetWindowText(hwndCrossoverRate, buffer, 10);
			//convert to an float
			g_fCrossoverRate = atof(buffer);
						//-----------first the number of balls
			GetWindowText(hwndMutationRate, buffer, 10);
			//convert to an float
			g_fMutateRate = atof(buffer);
						//-----------first the number of balls
			GetWindowText(hwndPopSize, buffer, 10);
			//convert to an float
			g_iPopSize = atoi(buffer);

			GetWindowText(hwndChromoLength, buffer, 10);
			//convert to an float
			g_iChromoLength = atoi(buffer);

			GetWindowText(hwndMaxGeneration, buffer, 10);
			//convert to an float
			g_iMaxGeneration = atoi(buffer);

			GetWindowText(hwndShowOrigin, buffer, 10);
			//convert to an float
			g_iShowOriginalLine= atoi(buffer);
		
			GetWindowText(hwndShowFixed, buffer, 10);
			//convert to an float
			g_iShowFixedLine = atoi(buffer);



          //send a custom message to the WindowProc so that
          //new balls are spawned
          PostMessage(g_hwnd, UM_SPAWN_NEW, NULL, NULL);

          EndDialog(hwnd, 0);

          return true;
        }

        break;
      }
    }

    break;

  }//end switch

  return false;
}
//-----------------------------------WinProc------------------------------------------
//
//------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	//device context for our window
	HDC				hdc;	
	PAINTSTRUCT     ps;

	//these hold the dimensions of the client window area
	static int cxClient, cyClient;

	//used to create the back buffer
	static HDC		hdcBackBuffer;
	static HBITMAP	hBitmap;
	static HBITMAP	hOldBitmap;
						  

	switch(msg)
	{	
    case WM_CREATE: 
		{
			 //get the instance handle so we can invoke the dialogue box
			 //when required
			 hInstance = ((LPCREATESTRUCT)lparam)->hInstance;
			//seed the random number generator
			srand((unsigned) time(NULL));

			//create the GA class
			g_pGABob = new CgaBob(g_fCrossoverRate ,
								            g_fMutateRate ,
							            	g_iPopSize ,
								            g_iChromoLength
								            );
			
			//get the size of the client window
			RECT rect;
			GetClientRect(hwnd, &rect);

			cxClient = rect.right;
			cyClient = rect.bottom;

			//create a surface for us to render to(backbuffer)
			hdcBackBuffer = CreateCompatibleDC(NULL);

			HDC hdc = GetDC(hwnd);

			hBitmap = CreateCompatibleBitmap(hdc,
											 cxClient,
											 cyClient);

			ReleaseDC(hwnd, hdc);

			hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);


		} 
	case UM_SPAWN_NEW:
		{
			g_pGABob->Stop();
			//delete our GA object
			delete g_pGABob;

			//create the GA class
			g_pGABob = new CgaBob(g_fCrossoverRate ,
								            g_fMutateRate ,
							            	g_iPopSize ,
								            g_iChromoLength
								            );
			bool bOrigin=false;
			bool bFixed=true;
			if(g_iShowFixedLine<=0)
			{
				bFixed=false;
			}
			if(g_iShowOriginalLine>0)
			{
				bOrigin=true;
			}
			g_pGABob->SetShowOption(bOrigin,bFixed);
	

		}
			
		break;
		//check key press messages
		case WM_KEYUP:
		{
			switch(wparam)
			{
				case VK_RETURN:
				{
					g_pGABob->Run(hwnd);

				}
					
				break;

				case VK_ESCAPE:
				{
					PostQuitMessage(0);
				}

				break;

				case VK_SPACE:
					{
						g_pGABob->Stop();
					}
					
					break;

				
			}//end switch
		}

		break;

		//has the user resized the client area?
		case WM_SIZE:
		{
			cxClient = LOWORD(lparam);
			cyClient = HIWORD(lparam);

			//resize the backbuffer accordingly
			SelectObject(hdcBackBuffer, hOldBitmap);

			HDC hdc = GetDC(hwnd);

			hBitmap = CreateCompatibleBitmap(hdc,
											cxClient,
											cyClient);

			ReleaseDC(hwnd, hdc);
			
			hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
		} 

		break;
		
		case WM_COMMAND:
		{
			switch(wparam)
			{
			case ID_SETTING_ARG:
				{
					 DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, OptionDialogProc);
				
				}
				break;
			case ID_SETTING_EXIT:
				{

				}
				break;
			}
		}
		break;
  
		case WM_PAINT: 
		{

			hdc = BeginPaint(hwnd, &ps);
			
			//fill our backbuffer with white
			BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, WHITENESS);
			
			//render the map and best route
			g_pGABob->Render(cxClient, cyClient, hdcBackBuffer);
			
			//now blit backbuffer to front
			BitBlt(hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);

			ReleaseDC(hwnd, hdc);
			
			EndPaint(hwnd, &ps);
		} 
			
		break;

		case WM_DESTROY: 
		{
			
			SelectObject(hdcBackBuffer, hOldBitmap);
			
			//clean up our backbuffer objects
			DeleteDC(hdcBackBuffer);
			DeleteObject(hBitmap);

			//delete our GA object
			delete g_pGABob;

			// kill the application, this sends a WM_QUIT message 
			PostQuitMessage(0);

   		} 
			
		break;

		default:break;

	}//end switch

	// default msg handler 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

}//end WinProc


//-----------------------------------WinMain-----------------------------------------
//	Entry point for our windows application
//-----------------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

	WNDCLASSEX winclass; 
	HWND	   hwnd;	 
	MSG		   msg;		 

	// first fill in the window class stucture
	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			    = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	  = WindowProc;
	winclass.cbClsExtra		  = 0;
	winclass.cbWndExtra		  = 0;
	winclass.hInstance		  = hinstance;
	winclass.hIcon			    = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		    = LoadCursor(NULL, IDC_ARROW); 
	winclass.hbrBackground	= NULL; 
	winclass.lpszMenuName	  = MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName	= szWindowClassName;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);


	// register the window class
	if (!RegisterClassEx(&winclass))
		return 0;

	// create the window
	if (!(hwnd = CreateWindowEx(NULL,									
								szWindowClassName,						
								szApplicationName,						
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					 			0,0,									
								WINDOW_WIDTH,WINDOW_HEIGHT,				
								NULL,									
								NULL,								
								hinstance,								
								NULL)))									
	return 0;
	
	g_hwnd = hwnd;

	ShowWindow(hwnd, ncmdshow);
	UpdateWindow(hwnd);

	//enter the message loop
	bool bDone = false;

	while(!bDone)
	{	
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{
			if( msg.message == WM_QUIT ) 
			{
				// Stop loop if it's a quit message
				bDone = true;
			} 

			else 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		
    //if the user has started the run update the GA and display
    //accordingly
    if (g_pGABob->Started())
    {
	    //update the gun
      g_pGABob->Epoch();

	    //this will call WM_PAINT 
	    InvalidateRect(hwnd, NULL, TRUE);
		  UpdateWindow(hwnd);
    }
					
					
	}//end while
	

  UnregisterClass( szWindowClassName, winclass.hInstance );
 
             
  return 0;                                   
             

} // end WinMain



