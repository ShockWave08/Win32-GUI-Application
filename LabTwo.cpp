// Author: Lee D. Daniel
// Course: ELET3260 - Adv Micoprocessors and Systems
// Date: 13 - 02 - 2020
// Program Name: JimmyDaLou

#include "resource.h"

using namespace std;

///////////////// Function Prototypes ////////////////////
LRESULT CALLBACK WindowProcedure( HWND, UINT, WPARAM, LPARAM); 
LRESULT CALLBACK DialogProcedure(HWND,UINT, WPARAM, LPARAM);                      // 
void RegisterDialogClass(HINSTANCE);												 //
void displayDialog(HWND);															 //
void AddDialogBox(HWND);
void addControls(HWND);
void openFile(HWND);
void openSrcFile(HWND);
void openLogFile(HWND);
void setTextBoxDisplay(char);
void processFile();									 //

// Global Variables:
HWND TextBox;
HWND inputBox;														 

/*
* @parameters
*
* hInst - A handle to the current instance of the application.																		 
* hPrevInst - A handle to the previous instance of the application																	     //
* args - The command line for the application, excluding the program name
* ncmdshow - 
*
*@ Function Description
* WinMqain is the user-provided entry point for a graphical Windows-based application.
* The WinMain should initialize the application, display its main window, and enter a message 
* retrieval-and-dispatch loop that is the top-level control structure for the remainder of the application's execution. 
* Terminate the message loop when it receives a WM_QUIT message.
*
*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow )
{
	WNDCLASSEXW wc  	= {0} ;
	HWND hwnd;
	MSG Msg 			= {0};;

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.hbrBackground 	= (HBRUSH)(COLOR_WINDOW+1) ;										 // A handle to the class background brush
	wc.hCursor 			= LoadCursor(NULL, IDC_ARROW); 										 // A handle to the class cursor. This member must be a handle to a cursor resource. 
	wc.hInstance 		= hInst;															 // A handle to the instance that contains the window procedure for the class.
	wc.lpszClassName 	= L"SimpleWindow";												 // A pointer to a null-terminated string or is an atom
	wc.lpszMenuName 	= MAKEINTRESOURCEW(IDR_SimpleMENU);
	wc.lpfnWndProc 		= WindowProcedure; 												 // A pointer to the window procedure	
	wc.cbWndExtra 		= 0;
	wc.cbClsExtra		= 0;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	
	if(!RegisterClassExW(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
	
	 
	RegisterDialogClass(hInst);														 // Registers any nonstandard window classes required by a screen saver's configuration dialog box.
	
	/*
	*  void CreateWindowA(
    *  lpClassName - A null-terminated string or a class atom created by a previous call to the RegisterClass or RegisterClassEx function
    *  lpWindowName - The window name.
    *  dwStyle - The style of the window being created.
    *  x - The initial horizontal position of the window
    *  y - The initial vertical position of the window
    *  nWidth - The width, in device units, of the window
    *  nHeight - The height, in device units, of the window
    *  hWndParent - A handle to the parent or owner window of the window being created
    *  hMenu - A handle to a menu, or specifies a child-window identifier depending on the window style.
    *  hInstance - A handle to the instance of the module to be associated with the window.
    *  lpParam - A pointer to a value to be passed to the window through the CREATESTRUCT structure (lpCreateParams member) pointed to by the lParam param of the WM_CREATE message.
	* 
	*@ Fucntion Description
	* Creates an overlapped, pop-up, or child window. It specifies the window class, window title, window style, 
	* and (optionally) the initial position and size of the window. 
	* The function also specifies the window's parent or owner, if any, and the window's menu.
	*
   */
	CreateWindowExW(WS_EX_CLIENTEDGE, L"SimpleWindow", L"LabOne", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 350,40,1000,500, NULL, NULL, hInst ,NULL );
	
	
	if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
	
	
	ShowWindow(hwnd, nCmdShow);
   	UpdateWindow(hwnd);
	
	/*
	* Retrieves a message from the calling thread's message queue. 
	* The function dispatches incoming sent messages until a posted message is available for retrieval. 
	*/
	while( GetMessage(&Msg, NULL, 0, 0) > 0)
	{
//		if(!IsDialogMessage(g_hToolbar, &Msg))
//        {
//            TranslateMessage(&Msg);
//            DispatchMessage(&Msg);
//        }
		
		TranslateMessage(&Msg);															// Translates virtual-key messages into character messages
		DispatchMessage(&Msg);															// Dispatches a message to a window procedure. 
	}
	return 0;
}


/*
* @parameters
* hWnd - a handle to the window
* msg - is the message code; for example, the WM_SIZE message indicates the window was resized.
* wp - contain additional data that pertains to the message.
* lp - contain additional data that pertains to the message.
*
* @ Program Description
* LRESULT is an integer value that your program returns to Windows. It contains your program's response to a particular message. 
*/
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		// Sent when the user selects a command item from a menu, when a control sends a notification message to its parent window, 
		// or when an accelerator keystroke is translated
		case WM_COMMAND:
			switch(wp)
			{
				case IDA_MENU_EXIT:
					DestroyWindow(hWnd);			// PostMessage(hwnd, WM_CLOSE, 0, 0);
					break;
			
				case IDA_MENU_ABOUT:
					displayDialog(hWnd);
					break;
				
				case ID_OPEN_FILE_BUTTON:
					openFile(hWnd);
					break;
					
				case ID_LOG_FILE_BUTTON:
					openLogFile(hWnd);
					break;
					
				case ID_SRC_FILE_BUTTON:
					openSrcFile(hWnd);
					break;
					
				case ID_PROCESS_FILE_BUTTON:
					processFile();
					break;
			 
			} // end inner switch
	        
		break;
		
		
		
		// Sent when an application requests that a window be created by calling the CreateWindowEx or CreateWindow function.
		case WM_CREATE:	
				addControls(hWnd);		
				HMENU hMenu, hSubMenu;
		
		        hMenu = CreateMenu();
		        hSubMenu = CreatePopupMenu();
			
				// File PopUp Menu
				AppendMenu(hSubMenu,MF_STRING,IDA_MENU_EXIT,"Exit");							// Appends a new item to the end of the specified menu bar, drop-down menu, submenu, or shortcut menu.
				AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu,"File");					//
				
				// About Menu
				AppendMenu(hMenu,MF_STRING,IDA_MENU_ABOUT,"About");						// 
			
				SetMenu(hWnd, hMenu);													// Assigns a new menu to the specified window.
			break;
			
		// Sent when a window is being destroyed.
		case WM_DESTROY:
        //	DestroyWindow(g_hToolbar);
			PostQuitMessage(0);
			break;
			
		default:
		
		// The function DefWindowProcW calls the default window procedure to provide default processing for any window messages that an application does not process. 
		// This function ensures that every message is processed.
		return DefWindowProcW(hWnd,msg,wp,lp);
	
	} // end outer switch

} // end 


/*
* @parameters
* hwnd - A handle to the dialog box.
* msg - The message.
* wp - Additional message-specific information.
* lp - Additional message-specific information.
*
* @ Function Description
* Application-defined callback function used with the CreateDialog and DialogBox families of functions. 
* It processes messages sent to a modal or modeless dialog box 
*
*/
LRESULT CALLBACK DialogProcedure(HWND hWnd,UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		// Sent when an application requests that a window be created by calling the CreateWindowEx or CreateWindow function.
		case WM_CREATE:
			AddDialogBox(hWnd);
			break;
		
		// Sent as a signal that a window or an application should terminate. 		
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
	} // end switch 
} // end LRESULT CALLBACK DialogProcedure


/*
* @parameters
* hInst - 
*  
* @ Program Description
* Application-defined callback function used with the CreateDialog and DialogBox families of functions. 
* It processes messages sent to a modal or modeless dialog box
* 
*/
void RegisterDialogClass(HINSTANCE hInst)
{
	WNDCLASSW dialog  = {0} ;

	dialog.hbrBackground = (HBRUSH)COLOR_WINDOW ;										 // A handle to the class background brush
	dialog.hCursor = LoadCursor(NULL, IDC_HAND);  										 // A handle to the class cursor. This member must be a handle to a cursor resource. 
	dialog.hInstance = hInst;															 // A handle to the instance that contains the window procedure for the class.
	dialog.lpszClassName = L"SimpleDialog";												 // A pointer to a null-terminated string or is an atom
	dialog.lpfnWndProc = DialogProcedure ;  											 // A pointer to the window procedure	

	RegisterClassW(&dialog);
} // end RegisterDialogClass


/*
* @parameters
*  hWnd - a handle to the dialogbox
*  
* @ Program Description
*
* 
*
*/
void displayDialog(HWND hWnd)
{
    CreateWindowW(L"SimpleDialog", L"DialogBox",  WS_VISIBLE | WS_OVERLAPPEDWINDOW , 400,200,500,300,hWnd,NULL,NULL,NULL);	
	
} // end displayDialog


/*
* @parameters
*  hWnd - a handle to the window dialogbox 
*  
* @ Function Description
*	This function adds a diaglob box to the window
*/
 void AddDialogBox(HWND hWnd)
{	
	/*
	*  void CreateWindowA(
    *  lpClassName - A null-terminated string or a class atom created by a previous call to the RegisterClass or RegisterClassEx function
    *  lpWindowName - The window name.
    *  dwStyle - The style of the window being created.
    *  x - The initial horizontal position of the window
    *  y - The initial vertical position of the window
    *  nWidth - The width, in device units, of the window
    *  nHeight - The height, in device units, of the window
    *  hWndParent - A handle to the parent or owner window of the window being created
    *  hMenu - A handle to a menu, or specifies a child-window identifier depending on the window style.
    *  hInstance - A handle to the instance of the module to be associated with the window.
    *  lpParam - A pointer to a value to be passed to the window through the CREATESTRUCT structure (lpCreateParams member) pointed to by the lParam param of the WM_CREATE message.
	* 
	*@ Fucntion Description
	* Creates an overlapped, pop-up, or child window. It specifies the window class, window title, window style, 
	* and (optionally) the initial position and size of the window. 
	* The function also specifies the window's parent or owner, if any, and the window's menu.
	*
   */
	CreateWindowW(L"static", L"User Name: Lee D. Daniel\n\nProgram Name: JimmyDaLou\n\nProgram Description: Simple GUI with Menu and DialogBox\n\nProgram Version Number: 1.0", WS_VISIBLE | WS_BORDER | WS_CHILD ,5,5,450,150,hWnd,NULL,NULL,NULL);	
}


/*
*/
void addControls(HWND hWnd)
{
	CreateWindowW(L"Button", L"Browse", WS_VISIBLE | WS_CHILD, 680, 12, 100, 30, hWnd, (HMENU)ID_OPEN_FILE_BUTTON, NULL, NULL);
	CreateWindowW(L"Button", L"Log", WS_VISIBLE | WS_CHILD, 15, 395, 100, 20, hWnd, (HMENU)ID_LOG_FILE_BUTTON, NULL, NULL);
	CreateWindowW(L"Button", L"Source", WS_VISIBLE | WS_CHILD, 350, 395, 100, 20, hWnd, (HMENU)ID_SRC_FILE_BUTTON, NULL, NULL);
	CreateWindowW(L"Button", L"Process", WS_VISIBLE | WS_CHILD, 680,395, 100, 20, hWnd, (HMENU)ID_PROCESS_FILE_BUTTON, NULL, NULL);
	inputBox = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER, 15, 12, 640, 30, hWnd, NULL, NULL, NULL);
	TextBox = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE| WS_BORDER | WS_VSCROLL, 15, 50, 780, 330, hWnd, NULL, NULL, NULL);
}

void openFile(HWND hWnd)
{	
	OPENFILENAME ofn;

	char file_name[100];
	HANDLE hf;              // file handle
	
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0]= '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "All files\0*.*\0HEX Files\0*.hex\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = "C:\\Users\\Sam-Fisher\\Desktop\\ELET3260 _.hex Files-20200219";													// can't place a value have no idea what folfer ist==ts gonna be in your laptop
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	// Display the Open dialog box. 	
	if (GetOpenFileName(&ofn) == TRUE)
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	
	SetWindowText(inputBox, ofn.lpstrFile);																// Changes the text of the specified window's title bar. 

} // end openFile

void openLogFile(HWND hWnd)
{	
	OPENFILENAME ofn;

	char file_name[100];
	HANDLE hf;              // file handle
	
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0]= '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "All files\0*.*\0Log Files\0*.log\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = "C:\\Users\\Sam-Fisher\\Desktop\\Results";										// can't place a value have no idea what folfer ist==ts gonna be in your laptop
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	// Display the Open dialog box. 	
	if (GetOpenFileName(&ofn) == TRUE)
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
		
//	setTextBoxDisplay(ofn.lpstrFile);

} // end openFile


void openSrcFile(HWND hWnd)
{	
	OPENFILENAME ofn;

	char file_name[100];
	HANDLE hf;              // file handle
	
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0]= '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "All files\0*.*\0Hex Files\0*.hex\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = "C:\\Users\\Sam-Fisher\\Desktop\\ELET3260 _.hex Files-20200219";													// can't place a value have no idea what folfer ist==ts gonna be in your laptop
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	// Display the Open dialog box. 	
	if (GetOpenFileName(&ofn) == TRUE)
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
		
//	setTextBoxDisplay(ofn.lpstrFile);
	
	SetWindowText(inputBox, ofn.lpstrFile);	

} // end openFile

 Display entire file in editbox 
void setTextBoxDisplay(char* path)
{
	FILE *file;
	file = fopen(path, "rb");
	fseek(file,0,SEEK_END);
	int _size = ftell(file);
	rewind(file);
	char *data = new char(_size+1);
	fread(data,_size,1,file);
	data[_size] = '\0';
	
	SetWindowText(TextBox,data);
}



void processFile() 
{
    std::string line = "";
	
	char address[5];
	int count = 1;
    std::string buffer = "";     // we should verify this
    std::ifstream data("C:\\Users\\Sam-Fisher\\Desktop\\ELET3260 _.hex Files-20200219\\PIC32.hex", ios::in | ios::app);
//    std::ifstream data(getWindowText(), ios::in | ios::app);
    std::ofstream txt ("C:\\Users\\Sam-Fisher\\Desktop\\Results\\Processed.txt", ios::out | ios::trunc);
    std::ofstream log ("C:\\Users\\Sam-Fisher\\Desktop\\Results\\Processing.log", ios::out | ios::trunc);
   
    
    if(log.is_open()) 
	{
		log.clear();
		
		if(txt.is_open())
		{
			txt.clear();
			txt << "Addresses" << "\t\t\t" << "Record Data\n\n" << endl;
			
			if(data.is_open())
			{
				 while(!data.eof()) 
				{
					while ( getline (data,line) )
				    {	
				    	log << "Processing line " << count << endl;
				    	//string top = 
				    	SetWindowText(TextBox, "Processing line ");
						log << "Stripping address from line " << count << endl;
						SetWindowText(TextBox, "Stripping address from line ");
						size_t temp  = line.copy(address,4,3);			// Address data
				    	address[temp] = '\0';
				    	log << "Address stripped" << endl;
				    	SetWindowText(TextBox, "Address stripped");
				    	log << "Stripping record data from line " << count << endl;
				    	SetWindowText(TextBox, "Stripping record data from line ");
				      	buffer = line.substr(9,-2);						// Record data
				      	log << "Record data stripped" << endl;
				      	SetWindowText(TextBox, "Record data stripped");
				      	txt << "  " << address << "\t\t\t" << buffer << endl;
				      	log << "Line " << count << " processed\n\n\n";
				      	SetWindowText(TextBox, "Line Processed!! ");
				      	buffer.clear();
				      	count++;
				    } // end inner while loop
			    	data.close();	
				} // end outer loop
	        }
	        txt.close();
	        log.close();
	        
		}
		else 
		{
	        std::cout << "unable to open log file" << std::endl;
	    }
        
    } 
	else 
	{
        std::cout << "unable to open log file" << std::endl;
    }
}
