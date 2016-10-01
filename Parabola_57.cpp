#include <math.h>
#include <windows.h>
#include "Parabola_57.h"

DRAWING_DATA gDrawData; // global data
void drawParabola(double a);
LRESULT CALLBACK DlgFocus(HWND hdlg,UINT mess,WPARAM more,LPARAM pos);

void reDraw(HWND hwnd)
{
  InvalidateRect(hwnd, NULL, 1);
}

void drawPoint(int x, int y)
{
  Ellipse(gDrawData.hdcMem,x-4,y-4,x+4,y+4);
}

void setupMenus(HWND hwnd)
{
  HMENU hMenu;
  
  hMenu=GetMenu(hwnd);

  switch (gDrawData.drawMode)
  {
    case READY_MODE :
      // enable the menu
      EnableMenuItem(hMenu, ID_PARABOLA,
                     MF_BYCOMMAND|MF_ENABLED);
      break;
    case DRAW_MODE :
    case DRAWN_MODE :
      // disable the menu
      EnableMenuItem(hMenu, ID_PARABOLA,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
  }
}

void setDrawMode(MODE mode, HWND hwnd)
{
  gDrawData.drawMode = mode;
  setupMenus(hwnd);
}

void createMemoryBitmap(HDC hdc)
{
  gDrawData.hdcMem = CreateCompatibleDC(hdc);
  gDrawData.hbmp = CreateCompatibleBitmap(hdc, 
    gDrawData.maxBoundary.cx, gDrawData.maxBoundary.cy);
  SelectObject(gDrawData.hdcMem, gDrawData.hbmp);
  PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
         gDrawData.maxBoundary.cy, PATCOPY);
}

void initialize(HWND hwnd, HDC hdc)
{
  gDrawData.hDrawPen=CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

  gDrawData.maxBoundary.cx = GetSystemMetrics(SM_CXSCREEN);
  gDrawData.maxBoundary.cy = GetSystemMetrics(SM_CYSCREEN);

  createMemoryBitmap(hdc);
  setDrawMode(READY_MODE, hwnd);
}

void cleanup()
{
  DeleteDC(gDrawData.hdcMem);
}

void reset(HWND hwnd)
{
  gDrawData.centre.x = gDrawData.centre.y = 0;

  gDrawData.drawMode = READY_MODE;

  PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
         gDrawData.maxBoundary.cy, PATCOPY);

  reDraw(hwnd);
  setupMenus(hwnd);
}

void plot_sympoint(int ex, int ey, COLORREF clr)
{
  int cx = gDrawData.centre.x;
  int cy = gDrawData.centre.y;

  SetPixel(gDrawData.hdcMem, ex+cx,cy-ey, clr);
  SetPixel(gDrawData.hdcMem, ex+cx,cy+ey, clr);
}

void addPoint(HWND hwnd, int x, int y)
{
  switch (gDrawData.drawMode)
  {
    case DRAW_MODE:
      /* the coordinates of the centre is stored 
         and the ellipse is drawn */
      SelectObject(gDrawData.hdcMem, gDrawData.hDrawPen);
      drawPoint(x,y);
      gDrawData.centre.x = x;
      gDrawData.centre.y = y;
      drawParabola(gDrawData.ilatusrectum);
      setDrawMode(DRAWN_MODE, hwnd);
      reDraw(hwnd);
      break;
    case DRAWN_MODE:
      MessageBox(hwnd,
       "Ellipse already drawn, now you can clear the area", 
          "Warning",MB_OK);
      break;
    default:
      break;
  }
}

void drawImage(HDC hdc)
{
  BitBlt(hdc, 0, 0, gDrawData.maxBoundary.cx, 
    gDrawData.maxBoundary.cy, gDrawData.hdcMem, 
    0, 0, SRCCOPY);
}

void processLeftButtonDown(HWND hwnd, int x, int y)
{
  addPoint(hwnd,x,y);
}

void processCommand(int cmd, HWND hwnd)
{
  int response;
  switch(cmd)
  {
    case ID_CLEAR:
      reset(hwnd);
      setDrawMode(READY_MODE, hwnd);
      break;
    case ID_PARABOLA:
      setDrawMode(DRAW_MODE, hwnd);
      DialogBox(NULL,"MyDB",hwnd,(DLGPROC)DlgFocus);
      break;
    case ID_EXIT:
        response=MessageBox(hwnd,
          "Quit the program?", "EXIT", 
          MB_YESNO);
        if(response==IDYES) 
            PostQuitMessage(0);
        break;
    default:
      break;
  }
}


int fparabola(double x,double y,int a)
{
	double z = y*y;
	z = z - (4*a*x);
	int b = (int) z+0.5;
	return b;
}


void drawParabola(double a)
{
  int x,y;
  x=0;
  y=0;
  plot_sympoint(x,y, RGB(0,0,0));

  if(a < 1)	
  {
	  double dparam = fparabola(x+1,y+(1/2),a);
	  while(x<400)
	  {
	  	x++;
		if(dparam > 0)
		{
		   dparam--;
		} 
		else
	    {
			dparam = dparam+(2*y)+1;
			y++;
		}
		plot_sympoint(x,y, RGB(0,0,0));			
	  }	
  }	
  else
  {
  	double dparam = fparabola(x+1/2,y+1,a);
  	while(y<a)
  	{
  		y++;
  		if(dparam > 0)
  		{
  			dparam = (2*y)+3;	
		}
		else
		{
			dparam = (2*y)+3 - (4*a);
			x++;
		}	
		plot_sympoint(x,y, RGB(0,0,0));			
	}
	dparam = fparabola(x+1,y+(1/2),a);
	while(x < 100)
	{
	  	x++;
		if(dparam > 0)
		{
		   dparam--;
		} 
		else
	    {
			dparam = dparam+2*y+1;
			y++;
		}
		plot_sympoint(x,y, RGB(0,0,0));			
	}
  }
}

LRESULT CALLBACK DlgFocus(HWND hdlg,UINT mess,WPARAM more,LPARAM pos)
{
  char str[20];
  switch(mess)
  {
    case WM_COMMAND:
      switch(more)
      {
        case ID_OK:
          GetDlgItemText(hdlg,ID_EB1,str,20);
          gDrawData.ilatusrectum=(long int)(atof(str))/2;
          if(gDrawData.ilatusrectum < 0 || gDrawData.ilatusrectum > 700)
          {
            MessageBox(hdlg,
              "Latus Recturm should be between 0 and 400.", 
              "Warning!",MB_ICONERROR);
          } 
          EndDialog(hdlg,TRUE);
          return 1;
          break;

        case ID_CANCEL:
          EndDialog(hdlg,TRUE);
          break;
      }
      break;
    default:
      break;
  }
  return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,
                         LPARAM lParam) 
{
  HDC hdc;
  PAINTSTRUCT ps;
  int x,y;

  switch(message)
  {
    case WM_CREATE:
      hdc = GetDC(hwnd);
      initialize(hwnd, hdc);
      ReleaseDC(hwnd, hdc);
      break;
    
    case WM_COMMAND:
      processCommand(LOWORD(wParam), hwnd);
      break;

    case WM_LBUTTONDOWN:
      x = LOWORD(lParam);
      y = HIWORD(lParam);
      processLeftButtonDown(hwnd, x,y);
      break;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      drawImage(hdc);
      EndPaint(hwnd, &ps);
      break;

    case WM_DESTROY:
      cleanup();
      PostQuitMessage(0);
      break;
  }
  // Call the default window handler
  return DefWindowProc(hwnd, message, wParam, lParam);
}


