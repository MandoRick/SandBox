typedef float imaginaryaxe;

extern MCUFRIEND_kbv tft;

class Mandelbrot {
 public:
   Mandelbrot() {
     createPaletteStripes();
     max_iterations = 64;
     escape_velocity = 100;
     working = false;
     background = tft.color565(128, 20, 200);
   }
   
   void createPaletteGrayscale() {
     for(int i=0;i<255;++i) {
       palette[i] = tft.color565(i, i, i);
     }
   }
   
   void createPaletteStripes() {
      for(int i=0;i<51;++i) {
        palette[i]     = tft.color565(239-i, 103-i, 51+i);
        palette[i+ 51] = tft.color565(190-i, 200-i, 65+i);
        palette[i+102] = tft.color565(254-i, 253-i, 223-i);
        palette[i+153] = tft.color565(0+i, 161-i, 162-i);
        palette[i+204] = tft.color565(0+i, 45+i, 40+i);
     }
  }
   
   void setParameters(imaginaryaxe x1, imaginaryaxe y1, imaginaryaxe x2, imaginaryaxe y2) {
     xmin = x1;
     ymin = y1;
     xmax = x2;
     ymax = y2;
   }
 
   void zoomIntoPoint(int x, int y, int zoom) {
     int height = tft.height();
     int width = tft.width();
 
      imaginaryaxe cx = xmin+y*(xmax-xmin)/(height-1);
      imaginaryaxe cy = ymin+x*(ymax-ymin)/(width-1);
      
      imaginaryaxe cwidthscale = (xmax-xmin) / zoom;
      imaginaryaxe cheightscale = (ymax-ymin) / zoom;
       
      setParameters(cx - cwidthscale / 2,  cx + cwidthscale / 2,     
          cy - cheightscale / 2, cy + cheightscale / 2);
   }

   void draw() {
     working = true;
     currentY = 0;
   }

   void update() {
     if (working) {
       tft.drawLine(0, currentY+1, tft.width(), currentY+1, 0);
       renderLine(currentY);      
       if (++currentY == tft.height()) {
         working = false;
       } 
     }
   }
    
    void computeArea (imaginaryaxe cx, imaginaryaxe cy, int max_iterations, uint16_t &colour) {
    
      int iteration=0;
      imaginaryaxe x1=0;
      imaginaryaxe y1=0;
      imaginaryaxe x2=0;
      imaginaryaxe y2=0;
      imaginaryaxe temp=0;

      while(true) {
        if (iteration >= max_iterations) {
          colour = background;
          return;
        } else if ((x2+y2) > escape_velocity){
          colour =  palette[(255 * iteration) / max_iterations];
          return;
        }
      
        temp = x2-y2+cx;
        y1 = (2*x1*y1)+cy;
        x1 = temp;
        
        x2 = x1*x1;
        y2 = y1*y1;
        
        ++iteration;
      }
      
  }

   void renderLine(int y) {
     uint16_t col;
     int width = tft.width();
     int height = tft.height();
     
     // We're working as landscape, so height & width are reversed
     imaginaryaxe deltaX = (xmax-xmin)/(height-1);  
     imaginaryaxe deltaY = (ymax-ymin)/(width-1);
     
     imaginaryaxe cx = xmin + y * deltaX;
     imaginaryaxe cy = ymin;

     for(int x=0;x<width;++x) {     
       computeArea(cx, cy, max_iterations, col);
       tft.drawPixel(x, y, col);  
    
       cy += deltaY;    
      }
   }
  
  void drawPGMData(const uint8_t *pBinaryData, long data_size) {
    const uint8_t *pData = pBinaryData;
    int width = tft.width();
    uint32_t data;
    int onbit = 0;
    int idx=0;
    int tracer = 0;
    
    for(long i=0;i<data_size;++i) {
      int y = i/width;
      
      if (tracer == 0) {
        // You may like this feature, but it slows everything down
        // a little too much for me
        //tft.drawLine(0, y+1, width, y+1, 0);
        tracer = 240/8;
      } else {
        --tracer;
      }  
    
      if (onbit == 0) {
        data = pgm_read_byte_near(pData) ^ 0xaa;
        onbit = 8;
        ++pData;
      }
      tft.drawPixel(i%width, y, data & 0x80 ? palette[0] : background);
      data <<= 1;
      --onbit;
    }
  }
   
 
   private:
   float xmin, ymin, xmax, ymax;
   int max_iterations;
   int escape_velocity;
   uint16_t palette[256];
   uint16_t background;
   boolean working;
   int currentY;

};
