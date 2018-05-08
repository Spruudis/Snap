#include <ap_fixed.h>
#include <ap_int.h>
#include <stdint.h>
#include <assert.h>

typedef ap_uint<8> pixel_type;
typedef ap_int<8> pixel_type_s;
typedef ap_uint<96> u96b;
typedef ap_uint<32> word_32;
typedef ap_ufixed<8,0, AP_RND, AP_SAT> comp_type;
typedef ap_fixed<10,2, AP_RND, AP_SAT> coeff_type;
typedef unsigned char uchar;

struct pixel_data {
	pixel_type blue;
	pixel_type green;
	pixel_type red;
};

const int background_r = 0;      ///Maybe needs to be adjusted
const int background_g = 0;      ///Maybe needs to be adjusted
const int background_b = 0;      ///Maybe needs to be adjusted

const int rangeArea = 10000; //////// TESTS TO DETERMINE
const int plusminus = 1000; /////// TESTS NEEDED
const int range_Colour = 10;
const int arraysize = 78;
const int h = 1080;
const int w = 1920;


struct my_buffer{

  void initialise(volatile uint32_t*& in_data){


    for(int j = 0; j < 2; j++){
  		for(int i = 0; i < w; i++){
                    #pragma HLS PIPELINE II=1
                    #pragma HLS LOOP_FLATTEN off
  			buff[i][j] = *in_data++;
  		}
  	}

    row_in = 2;
    row_out_0 = -1;
  	row_out_1 = 0;
  	row_out_2 = 1;

    output_row = 0;
    output_column = 0;

  }




  unsigned int get_average(volatile uint32_t*& in_data){
    if(output_column== w){
      output_column = 0; // resets column counter
      output_row++;
//      std::cout<<"output row++"<<std::endl;

      row_in = (row_in + 1) % 4;   // points to buffer to write
  		row_out_0 = (row_out_0 + 1) % 4;	//points to buffers to read
  		row_out_1 = (row_out_1 + 1) % 4;
  		row_out_2 = (row_out_2 + 1) % 4;
    }

//    if(output_column == w/2 && output row = )

    if(output_row==0){

      if(output_column==0){
        L  = buff[0][row_out_1];   ////!!!! M DOESNT POINT TO THE RIGHT PIXEL AT THE START AND END
    		M  = buff[1][row_out_1];    ////// MAYBE SOVE WITH LOGIC IN THE GET_PURE FUNCTION THAT CHACKS OUTPUT COLUMN
    		R  = buff[2][row_out_1];
    		LD = buff[0][row_out_2];
    		D  = buff[1][row_out_2];
    		RD = buff[2][row_out_2];
      }

      else if(output_column==w-1 || output_column==1){
        //do nothing
      }

      else{
        L = M;
        M = R;
        R = buff[output_column+1][row_out_1];
        LD = D;
        D = RD;
        RD = buff[output_column+1][row_out_1];
      }
      //do average here
      unsigned char L_in_r = L & 0xFF;
      unsigned char L_in_g = (L >> 8) & 0xFF;
      unsigned char L_in_b = (L >> 16) & 0xFF;

      unsigned char M_in_r = M & 0xFF;
      unsigned char M_in_g = (M >> 8) & 0xFF;
      unsigned char M_in_b = (M >> 16) & 0xFF;

      unsigned char R_in_r = R & 0xFF;
      unsigned char R_in_g = (R >> 8) & 0xFF;
      unsigned char R_in_b = (R >> 16) & 0xFF;

      unsigned char LD_in_r = LD & 0xFF;
      unsigned char LD_in_g = (LD >> 8) & 0xFF;
      unsigned char LD_in_b = (LD >> 16) & 0xFF;

      unsigned char D_in_r = D & 0xFF;
      unsigned char D_in_g = (D >> 8) & 0xFF;
      unsigned char D_in_b = (D >> 16) & 0xFF;

      unsigned char RD_in_r = RD & 0xFF;
      unsigned char RD_in_g = (RD >> 8) & 0xFF;
      unsigned char RD_in_b = (RD >> 16) & 0xFF;

      unsigned char average_r = (L_in_r + M_in_r + R_in_r + LD_in_r + D_in_r + RD_in_r)/6;
      unsigned char average_g = (L_in_g + M_in_g + R_in_g + LD_in_g + D_in_g + RD_in_g)/6;
      unsigned char average_b = (L_in_b + M_in_b + R_in_b + LD_in_b + D_in_b + RD_in_b)/6;

      unsigned int output = average_r | (average_g << 8) | (average_b << 16);

      buff[output_column][row_in]=*in_data++;
      output_column++;

      return output;
    }


    else if(output_row==h-2){

      if(output_column==0){
        LU = buff[0][row_out_0];
        U  = buff[1][row_out_0];
        RU = buff[2][row_out_0];
    		L  = buff[0][row_out_1]; //FIGURE OUT WHICH ROW_OUT IS ON WHICH LINE
    		M  = buff[1][row_out_1]; //////I THINK ! IS ALLWAYS IN THE MIDDLE
    		R  = buff[2][row_out_1];
        LD = buff[0][row_out_2];
    		D  = buff[1][row_out_2];
    		RD = buff[2][row_out_2];
      }
      else if(output_column==1 || output_column==w-1){
        //do nothing
      }
      else{
        LU = U;
        U = RU;
        RU = buff[output_column+1][row_out_0];//FIGURE OUT WHICH ROW_OUT IS ON WHICH LINE
        L = M;
        M = R;
        R = buff[output_column+1][row_out_1];//FIGURE OUT WHICH ROW_OUT IS ON WHICH LINE
        LD = D;
        D = RD;
        RD = buff[output_column+1][row_out_2];
      }
      unsigned char LU_in_r = LU & 0xFF;
      unsigned char LU_in_g = (LU >> 8) & 0xFF;
      unsigned char LU_in_b = (LU >> 16) & 0xFF;

      unsigned char U_in_r = U & 0xFF;
      unsigned char U_in_g = (U >> 8) & 0xFF;
      unsigned char U_in_b = (U >> 16) & 0xFF;

      unsigned char RU_in_r = RU & 0xFF;
      unsigned char RU_in_g = (RU >> 8) & 0xFF;
      unsigned char RU_in_b = (RU >> 16) & 0xFF;

      unsigned char L_in_r = L & 0xFF;
      unsigned char L_in_g = (L >> 8) & 0xFF;
      unsigned char L_in_b = (L >> 16) & 0xFF;

      unsigned char M_in_r = M & 0xFF;
      unsigned char M_in_g = (M >> 8) & 0xFF;
      unsigned char M_in_b = (M >> 16) & 0xFF;

      unsigned char R_in_r = R & 0xFF;
      unsigned char R_in_g = (R >> 8) & 0xFF;
      unsigned char R_in_b = (R >> 16) & 0xFF;

      unsigned char LD_in_r = LD & 0xFF;
      unsigned char LD_in_g = (LD >> 8) & 0xFF;
      unsigned char LD_in_b = (LD >> 16) & 0xFF;

      unsigned char D_in_r = D & 0xFF;
      unsigned char D_in_g = (D >> 8) & 0xFF;
      unsigned char D_in_b = (D >> 16) & 0xFF;

      unsigned char RD_in_r = RD & 0xFF;
      unsigned char RD_in_g = (RD >> 8) & 0xFF;
      unsigned char RD_in_b = (RD >> 16) & 0xFF;

      unsigned char average_r = (LU_in_r + U_in_r + RU_in_r + L_in_r + M_in_r + R_in_r + LD_in_r + D_in_r + RD_in_r)/9;
      unsigned char average_g = (LU_in_g + U_in_g + RU_in_g + L_in_g + M_in_g + R_in_g + LD_in_g + D_in_g + RD_in_g)/9;
      unsigned char average_b = (LU_in_b + U_in_b + RU_in_b + L_in_b + M_in_b + R_in_b + LD_in_b + D_in_b + RD_in_b)/9;

      unsigned int output = average_r | (average_g << 8) | (average_b << 16);

      output_column++;
      return output;

    }


    else if(output_row==h-1){

      if(output_column==0){
        LU = buff[0][row_out_0];
        U  = buff[1][row_out_0];
        RU = buff[2][row_out_0];
    		L  = buff[0][row_out_1]; //FIGURE OUT WHICH ROW_OUT IS ON WHICH LINE
    		M  = buff[1][row_out_1]; //////I THINK ! IS ALLWAYS IN THE MIDDLE
    		R  = buff[2][row_out_1];
      }

      else if(output_column==1||output_column==w-1){
      //do nothing
      }

      else{
        LU = U;
        U = RU;
        RU = buff[output_column+1][row_out_0];//FIGURE OUT WHICH ROW_OUT IS ON WHICH LINE
        L = M;
        M = R;
        R = buff[output_column+1][row_out_1];//FIGURE OUT WHICH ROW_OUT IS ON WHICH LINE
      }

      unsigned char LU_in_r = LU & 0xFF;
      unsigned char LU_in_g = (LU >> 8) & 0xFF;
      unsigned char LU_in_b = (LU >> 16) & 0xFF;

      unsigned char U_in_r = U & 0xFF;
      unsigned char U_in_g = (U >> 8) & 0xFF;
      unsigned char U_in_b = (U >> 16) & 0xFF;

      unsigned char RU_in_r = RU & 0xFF;
      unsigned char RU_in_g = (RU >> 8) & 0xFF;
      unsigned char RU_in_b = (RU >> 16) & 0xFF;

      unsigned char L_in_r = L & 0xFF;
      unsigned char L_in_g = (L >> 8) & 0xFF;
      unsigned char L_in_b = (L >> 16) & 0xFF;

      unsigned char M_in_r = M & 0xFF;
      unsigned char M_in_g = (M >> 8) & 0xFF;
      unsigned char M_in_b = (M >> 16) & 0xFF;

      unsigned char R_in_r = R & 0xFF;
      unsigned char R_in_g = (R >> 8) & 0xFF;
      unsigned char R_in_b = (R >> 16) & 0xFF;

      unsigned char average_r = (LU_in_r + U_in_r + RU_in_r + L_in_r + M_in_r + R_in_r)/6;
      unsigned char average_g = (LU_in_g + U_in_g + RU_in_g + L_in_g + M_in_g + R_in_g)/6;
      unsigned char average_b = (LU_in_b + U_in_b + RU_in_b + L_in_b + M_in_b + R_in_b)/6;

      unsigned int output = average_r | (average_g << 8) | (average_b << 16);

      output_column++;

      return output;

    }


    else{
//    	std::cout<<"starting row mid"<<std::endl;
      if(output_column==0){
        LU = buff[0][row_out_0];
        U  = buff[1][row_out_0];
        RU = buff[2][row_out_0];
    	L  = buff[0][row_out_1]; //FIGURE OUT WHICH ROW_OUT IS ON WHICH LINE
    	M  = buff[1][row_out_1]; //////I THINK ! IS ALLWAYS IN THE MIDDLE
    	R  = buff[2][row_out_1];
        LD = buff[0][row_out_2];
    	D  = buff[1][row_out_2];
    	RD = buff[2][row_out_2];
      }
      else if(output_column==1 || output_column==w-1){
        //do nothing
      }
      else{
        LU = U;
        U = RU;
        RU = buff[output_column+1][row_out_0];//FIGURE OUT WHICH ROW_OUT IS ON WHICH LINE
        L = M;
        M = R;
        R = buff[output_column+1][row_out_1];//FIGURE OUT WHICH ROW_OUT IS ON WHICH LINE
        LD = D;
        D = RD;
        RD = buff[output_column+1][row_out_2];
      }

      unsigned char LU_in_r = LU & 0xFF;
      unsigned char LU_in_g = (LU >> 8) & 0xFF;
      unsigned char LU_in_b = (LU >> 16) & 0xFF;

      unsigned char U_in_r = U & 0xFF;
      unsigned char U_in_g = (U >> 8) & 0xFF;
      unsigned char U_in_b = (U >> 16) & 0xFF;

      unsigned char RU_in_r = RU & 0xFF;
      unsigned char RU_in_g = (RU >> 8) & 0xFF;
      unsigned char RU_in_b = (RU >> 16) & 0xFF;

      unsigned char L_in_r = L & 0xFF;
      unsigned char L_in_g = (L >> 8) & 0xFF;
      unsigned char L_in_b = (L >> 16) & 0xFF;

      unsigned char M_in_r = M & 0xFF;
      unsigned char M_in_g = (M >> 8) & 0xFF;
      unsigned char M_in_b = (M >> 16) & 0xFF;

      unsigned char R_in_r = R & 0xFF;
      unsigned char R_in_g = (R >> 8) & 0xFF;
      unsigned char R_in_b = (R >> 16) & 0xFF;

      unsigned char LD_in_r = LD & 0xFF;
      unsigned char LD_in_g = (LD >> 8) & 0xFF;
      unsigned char LD_in_b = (LD >> 16) & 0xFF;

      unsigned char D_in_r = D & 0xFF;
      unsigned char D_in_g = (D >> 8) & 0xFF;
      unsigned char D_in_b = (D >> 16) & 0xFF;

      unsigned char RD_in_r = RD & 0xFF;
      unsigned char RD_in_g = (RD >> 8) & 0xFF;
      unsigned char RD_in_b = (RD >> 16) & 0xFF;

      unsigned char average_r = (LU_in_r + U_in_r + RU_in_r + L_in_r + M_in_r + R_in_r + LD_in_r + D_in_r + RD_in_r)/9;
      unsigned char average_g = (LU_in_g + U_in_g + RU_in_g + L_in_g + M_in_g + R_in_g + LD_in_g + D_in_g + RD_in_g)/9;
      unsigned char average_b = (LU_in_b + U_in_b + RU_in_b + L_in_b + M_in_b + R_in_b + LD_in_b + D_in_b + RD_in_b)/9;

      unsigned int output = average_r | (average_g << 8) | (average_b << 16);

      output_column++;
      buff[output_column][row_in]=*in_data++;

      return output;
    }

  }




  // To be called after average
  unsigned char get_real(){
    if(output_column == 0){
      return L;
    }
    else if(output_column == w-1){
      return R;
    }
    else return M;
  }





  unsigned int buff[1920][4];


  unsigned int LU;
  unsigned int U;
  unsigned int RU;
  unsigned int L;
  unsigned int M;
  unsigned int R;
  unsigned int LD;
  unsigned int D;
  unsigned int RD;

  uchar row_in;   // points to buffer to write
  uchar row_out_0;	//points to buffers to read
  uchar row_out_1;
  uchar row_out_2;

  int output_row, output_column;
};



bool compare_pixels(ap_int<10> in_r, ap_int<10> in_g, ap_int<10> in_b, ap_int<10> reference_r, ap_int<10> reference_g, ap_int<10> reference_b);



void import(pixel_type pixel_array[]);
void initialise255(pixel_type pixel_array[]);
void initialise0(int pixel_array[]);

bool border_detect (int& pixel_count, unsigned int& current, volatile uint32_t* in_data, volatile uint32_t* out_data);



void add_to_Array(const pixel_type& red,const pixel_type& green,const pixel_type& blue, pixel_type array_add[]);
bool check_array(pixel_type array[], const pixel_type & in_r, const pixel_type & in_g, const pixel_type & in_b, int & position, bool skip_whites);


int del_from_Array(const pixel_type& red,const pixel_type& green,const pixel_type& blue, pixel_type array_del[]);

void pixel_check(pixel_type on_table[], pixel_type possible_candidates[], pixel_type card_presets[], int& pos_PC, int& pos_CP, int possible_candidates_count[],const pixel_type& in_r,const pixel_type& in_g,const pixel_type& in_b);


bool gameplaying(pixel_type& earliestnum, pixel_type& previousnum, pixel_type& presentnum, pixel_type& numberofcards, bool& noroyalties);
void resetdeck(pixel_type& earliestnum, pixel_type& previousnum, pixel_type& presentnum, pixel_type& numberofcards, bool& noroyalties);
void snap();
void enter_Pixel(unsigned int& current,volatile uint32_t* out_data, volatile uint32_t* in_data,pixel_type possible_candidates[],int possible_candidates_count[],pixel_type on_table[],pixel_type card_presets[]);


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

    ///////    ///////           ///////        ///////       ///////          //////
  //////  //////  /////         ///   ///        /////         ////////         ////
  /////    ////    /////       ///     ///       /////         ////  ////       ////
  /////     //     /////      /////////////      /////         ////   ////      ////
  /////            /////      ////     ////      /////         ////    /////    ////
  /////            /////      ////     ////      /////         ////      /////  ////
  /////            /////      ////     ////      /////         ////       ///// ////
  /////           ///////    //////   //////    ///////       //////        /////////







bool Game_main(volatile uint32_t* in_data, volatile uint32_t* out_data, int w, int h, int parameter_1, pixel_type card_presets[arraysize]){
/////////////////////////////////////////////////////INITIALISATIONS/////////////////////////////////////////////////////////////////
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=parameter_1
#pragma HLS INTERFACE s_axilite port=w
#pragma HLS INTERFACE s_axilite port=h

#pragma HLS INTERFACE m_axi depth=2073600 port=in_data offset=slave // This will NOT work for resolutions higher than 1080p
#pragma HLS INTERFACE m_axi depth=2073600 port=out_data offset=slave

///////////////////////////////////////////////////INITIALISATIONS//////////////////////////////////////////////////////////////////
	/////////////////////////////ARRAYS///////////////////////////
	pixel_type on_table[arraysize];
	pixel_type possible_candidates[arraysize];
	int possible_candidates_count[52];

	/////////////////////////////GAMEPLAYING VARIABLES AND CONSTS//////////////////////////////
  bool detect_cards = false;
  bool snap = false;
	bool noroyalties = false;
	const int numOfPixels = w*h;
	pixel_type earliestnum=16, previousnum=15, numberofcards=0;
	int pixelCounter;

	/////////////////////////////ARRAY INITIALISATIONS//////////////////////////////
  //import(card_presets);
  initialise255(on_table);

	/////////////////////////////COLOUR DOOOS/////////////////////////////////////////
	unsigned int current;
	unsigned char out;

///////////////////////////////////////////////////FIN INITIALISATIONS//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





	my_buffer buffer;
	buffer.initialise(in_data);

	for(int i = 0; i<h; i++){
		for(int j = 0; j<w; j++){
			*out_data++ = buffer.get_average(in_data);
		}
	}


 //End of game
}


















//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//BEFORE BORDER DETECTION SET UP ALGORITHM WHICH VARIES BETWEEN BORDER DETECTION AND CARD DETECTION ACCURATELY
//////////////////////BORDER DETECTION///////////////////


bool border_detect (int& pixel_count, unsigned int& current, volatile uint32_t* in_data, volatile uint32_t* out_data) {
	int counter1 = 1, counter2 = 1;
	pixel_count = 0;


	bool l1 = false;
	bool l2 = false;
	bool l3 = false;
	bool r1 = false;
	bool r2 = false;
	bool r3 = false;

  unsigned char in_r;
	unsigned char in_b;
	unsigned char in_g;


	for(int i = 0; i<1080; i++){ //counts height
		current = *in_data;


		in_r = current & 0xFF;
		in_g = (current >> 8) & 0xFF;
		in_b = (current >> 16) & 0xFF;






		if(i==0 || i==1079){


			if(!compare_pixels(in_r, in_g, in_b, background_r, background_g, background_b)){
				l1 = true;
			}
			counter1 = 2;
			current = *in_data++;
			*out_data++ = current;
			pixel_count++;

			for(int count1 = 0; count1 < 19; count1++){

				for(int count2 = 0; count2 < 101; count2++){
					current = *in_data++;
					*out_data++ = current;
          pixel_count++;
				}

				in_r = current & 0xFF;
				in_g = (current >> 8) & 0xFF;
				in_b = (current >> 16) & 0xFF;

				if(!compare_pixels(in_r, in_g, in_b, background_r, background_g, background_b)){
					if(counter1==1){l1=true;}
					else if(counter1==2){l2=true;}
					else if(counter1==3){l3=true;}
				}
				else{
					if(counter1==1){l1=false;}
					else if(counter1==2){l2=false;}
					else if(counter1==3){l3=false;}
				}
				counter1++;
				if(counter1 == 4){counter1=1;}
				if(l1&&l2&&l3){
					std::cout<<"Triggered"<< std::endl;
          return true;
					//TRIGGER CARD DETECTION
				}

			}
			i++;

      l1 = false;
			l2 = false;
			l3 = false;
		}








		else if(i%83==0){

			if(!compare_pixels(in_r, in_g, in_b, 0, 0, 0)){    //maybe pack this as a separate function
				if(counter1==1){l1=true;}
				else if(counter1==2){l2=true;}
				else if(counter1==3){l3=true;}
//				else if(counter2==1){r1=true;}
//				else if(counter2==2){r2=true;}
//				else if(counter2==3){r3=true;}
			}
			else{
				if(counter1==1){l1=false;}
				else if(counter1==2){l2=false;}
				else if(counter1==3){l3=false;}
//				else if(counter2==1){r1=false;}
//				else if(counter2==2){r2=false;}
//				else if(counter2==3){r3=false;}
			}
			counter1++;
			if(counter1 == 4){counter1=1;}

			if((l1&&l2&&l3)||(r1&&r2&&r3)){
				std::cout<<"Triggered"<< std::endl;
        return true;
				//TRIGGER CARD DETECTION in NEXT FRAME
			}


      current = *in_data++;
      *out_data++ = current;
      pixel_count++;

			for(int j = 0; j<1919; j++){
				current = *in_data++;
				*out_data++ = current;
				pixel_count++;
			}

			in_r = current & 0xFF;
			in_g = (current >> 8) & 0xFF;
			in_b = (current >> 16) & 0xFF;

			if(!compare_pixels(in_r, in_g, in_b, 0, 0, 0)){
//				if(counter1==1){l1=true;}
//				else if(counter1==2){l2=true;}
//				else if(counter1==3){l3=true;}
			  if(counter2==1){r1=true;}
				else if(counter2==2){r2=true;}
				else if(counter2==3){r3=true;}
			}
			else{
				if(counter2==1){r1=false;}
				else if(counter2==2){r2=false;}
				else if(counter2==3){r3=false;}
			}
			counter2++;
			if(counter2 == 4){counter2=1;}

			if((l1&&l2&&l3)||(r1&&r2&&r3)){
				std::cout<<"Triggered"<< std::endl;
        return true;
				//TRIGGER CARD DETECTION IN NEXT FRAME (SET OVERARCHING CONTROLLING BOOL TO DO CARD COUNTING)
			}

		}




		else{
			for(int j = 0; j<1920; j++){
				current = *in_data++;
				*out_data++ = current;
				pixel_count++;

			}
		}



	}
  return false;

}






//////////////////////////////////////////////////





	/////////////////////////////////////////////////////
	//Logic to perform on each pixel within range
	////////////////////////////////////////////////////
	//////////////pixel_type card_presets[arraysize];  			//white signifies empty slot///////////////////////////////////
  ////////////////pixel_type on_table[arraysize]; //white signifies end, checked frequently///////////////////////////////////          MOVED TO OUTSIDE, E.G. TOP OF CODE, INITIALISED AT THE START
  /////////////////pixel_type possible_candidates[arraysize]; //checked frequently////////////////////////////////////////////////
  ///////////////int possible_candidates_count[52]; //leave as pixel type or allocate more bits for counting?/////////////////






bool check_array(pixel_type array[], const pixel_type & in_r, const pixel_type & in_g, const pixel_type & in_b, int & position, bool skip_whites){///position can't go above 255
	position = 0;
	while(position<arraysize){
		if(array[position] == 255 && array[position + 1] == 255 && array[position + 2] == 255){
			if(skip_whites){
				position = position +3;
			}
			else{
				return false;
			}
		}
		else if(compare_pixels(in_r, in_g, in_b, array[position], array[position+1], array[position+2])){
			return true;
		}
		else{
			position = position + 3;
		}
	}
}





bool compare_pixels(ap_int<10> in_r, ap_int<10> in_g, ap_int<10> in_b, ap_int<10> reference_r, ap_int<10> reference_g, ap_int<10> reference_b){

//	ap_int<9> upper_r = 255, lower_r = 0, upper_g = 255, lower_g = 0, upper_b = 255, lower_b = 0;
//	reference_r-range = lower_r;
//	reference_r+range = upper_r;
//	reference_g-range = lower_g;
//	reference_g+range = upper_g;
//	reference_b-range = lower_b;
//	reference_b+range = upper_b;
	//if(reference_r<= 255-range & reference_g<= 255-range & reference_b<= 255-range ){
	if((in_r>reference_r-range_Colour) && (in_r<reference_r+range_Colour) && (in_g>reference_g-range_Colour) && (in_g<reference_g+range_Colour) && (in_b>reference_b-range_Colour) && (in_b<reference_b+range_Colour)){
		return true;
	}
//	else{
//		return false;
//	}
	//}
//	else if(in_r>=reference_r-range && in_g>=reference_g-range  && in_b>=reference_b-range){
//		return false;
//	}
	else{
		return false;
	}

	//I chose 7 as an arbitrary number for range, needs experimentation, card colour values
}



//void import(pixel_type pixel_array[]){
//  std::ifstream infile;
//  infile.open("pixels.txt");         //////////
//  pixel_type n;											///////////     IMPORT FUNCTION
//  int i = 0;												///////////			NEIL
//  while(infile >> n){								///////	///
//    pixel_a[i] = n;
//    i++;
//  }
//}

void initialise255(pixel_type pixel_array[]){     //////////
  for(int i = 0; i < arraysize; i++){									 //////////  INITIALISE 255
    pixel_array[i] = 255;
  }
}

void initialise0(int pixel_array[]){				//////////  INITIALISE 0
  for(int i = 0; i < arraysize/3; i++){										/////////
    pixel_array[i] = 0;
  }
}

void add_to_Array(const pixel_type& red,const pixel_type& green,const pixel_type& blue, pixel_type array_add[]){
	int i = 0;
  while(i < arraysize){
    if(array_add[i] == 255 && array_add[i + 1] == 255 && array_add[i + 2] == 255){
      array_add[i] = red;
      array_add[i + 1] = green;
      array_add[i + 2] = blue;
      i += arraysize;
    }
    else{
      i += 3;
    }
  }
}


int del_from_Array(const pixel_type& red,const pixel_type& green,const pixel_type& blue, pixel_type array_del[]){
  int i = 0;
 	int z = arraysize;
  while(i < arraysize){
    if(array_del[i] == red && array_del[i + 1] == green && array_del[i + 2] == blue){
      z = i;
      array_del[i] = 255;
      array_del[i + 1] = 255;
      array_del[i + 2] = 255;
      i += arraysize;
    }
    else if(compare_pixels(red, green, blue, array_del[i], array_del[i+1], array_del[i+2])){
			z = i;
      array_del[i] = 255;
      array_del[i + 1] = 255;
      array_del[i + 2] = 255;
      i += arraysize;
		}
    else{
      i += 3;
    }
  }
  return z;
}



//////////////////////////////////////////////////////////////////GAME PLAYING ALGORITHM////////////////////////////////////////////////////////////////////////////////////////////////////////////////





bool gameplaying(pixel_type& earliestnum, pixel_type& previousnum, pixel_type& presentnum, pixel_type& numberofcards, bool& noroyalties){
	if (noroyalties) {
		if (numberofcards>0) {
			if (presentnum == 1 || presentnum == 2 || presentnum == 3 || presentnum == 4 || presentnum == 5 || presentnum == 6 || presentnum == 7 || presentnum == 8 || presentnum == 9 || presentnum == 10) {
				numberofcards--;
			}
		}
		if (numberofcards == 0) {
			std::cout << "no royalties ended" << std::endl;
			//resetdeck(earliestnum, previousnum, presentnum, numberofcards, noroyalties);
			return true;
		}
	}

	if (presentnum == previousnum) {
		snap();
		//resetdeck(earliestnum, previousnum, presentnum, numberofcards, noroyalties);
		return true;
	}
	else if (presentnum == earliestnum) {
		snap();
		//resetdeck(earliestnum, previousnum, presentnum, numberofcards, noroyalties);
		return true;
	}

	if(presentnum == 7){
		snap();
		//resetdeck(earliestnum, previousnum, presentnum, numberofcards, noroyalties);
		return true;
	}
	else if(presentnum == 11){
		noroyalties = true;
		numberofcards = 1;
		std::cout << "no royalties" <<std::endl;
	}
	else if(presentnum == 12){
		noroyalties = true;
		numberofcards = 2;
		std::cout << "no royalties" <<std::endl;
	}
	else if(presentnum == 13){
		noroyalties = true;
		numberofcards = 3;
		std::cout << "no royalties" <<std::endl;
	}
	else if(presentnum == 1){
		noroyalties = true;
		numberofcards = 4;
		std::cout << "no royalties" <<std::endl;
	}


	earliestnum = previousnum;
	previousnum = presentnum;
  return false;
}


void resetdeck(pixel_type& earliestnum, pixel_type& previousnum, pixel_type& presentnum, pixel_type& numberofcards, bool& noroyalties){
	presentnum = 14; //special values reserved. The values mean nothing
	previousnum = 15;
	earliestnum = 16;
	numberofcards = 0;
	noroyalties = false;
}

void snap(){
	std::cout<<"snap bitches"<<std::endl;
}


void pixel_check(pixel_type on_table[], pixel_type possible_candidates[], pixel_type card_presets[],int& pos_PC, int& pos_CP, int possible_candidates_count[],const pixel_type& in_r,const pixel_type& in_g,const pixel_type& in_b){
	if(check_array(on_table, in_r, in_g, in_b, pos_PC, false)){
		//move on to next pixel, exit function
	}

	else if(check_array(possible_candidates, in_r, in_g, in_b, pos_PC, false)){
		possible_candidates_count[pos_PC/3] = possible_candidates_count[pos_PC/3] + 1;
	}


	else if(check_array(card_presets, in_r, in_g, in_b, pos_CP, true)){
		possible_candidates[pos_PC] = card_presets[pos_CP];
		possible_candidates[pos_PC + 1]  = card_presets[pos_CP + 1];
		possible_candidates[pos_PC + 2] = card_presets[pos_CP + 2];
		possible_candidates_count[pos_PC/3] = possible_candidates_count[pos_PC/3] + 1;

	}
}

void enter_Pixel(unsigned int& current, volatile uint32_t* out_data, volatile uint32_t* in_data, pixel_type possible_candidates[], int possible_candidates_count[], pixel_type on_table[], pixel_type card_presets[]){
	/*initialise255(possible_candidates);
	initialise0(possible_candidates_count);

	unsigned char in_r;
	unsigned char in_b;
	unsigned char in_g;


	for(int j = 0; j<3; j++){
		for(int i = 0; i < w; i++){
				#pragma HLS PIPELINE II=1
				#pragma HLS LOOP_FLATTEN off
			row[i][j] = *in_data++;
		}
	}

	uchar LU;
	uchar U;
	uchar RU;
	uchar L;
	uchar M;
	uchar R;
	uchar LD;
	uchar D;
	uchar RD;

	uchar cur_row_in = 3;   // points to buffer to write
	uchar cur_row_out_0 = 0;	//points to buffers to read
	uchar cur_row_out_1 = 1;
	uchar cur_row_out_2 = 2;


	for(int i = 0; i<h-3; i++){

		LU = row[0][cur_row_out_0];
		U  = row[1][cur_row_out_0];
		RU = row[2][cur_row_out_0];
		L  = row[0][cur_row_out_1];
		M  = row[1][cur_row_out_1];
		R  = row[2][cur_row_out_1];
		LD = row[0][cur_row_out_2];
		D  = row[1][cur_row_out_2];
		RD = row[2][cur_row_out_2];

		uchar cur_row_in = 3;   // points to buffer to write
		uchar cur_row_out_0 = 0;	//points to buffers to read
		uchar cur_row_out_1 = 1;
		uchar cur_row_out_2 = 2;

		for(int i = 0; i < w; i++){

			//buffer
			row[i][cur_row_in]=*in_data;
			in_data++;
			if((j == 0) || (j == h-1) || (i == 0) || (i == w-1)){ //jak cos to zmienic
				out = 0;
				unsigned int output = out | (out << 8) | (out << 16);
				*out_data++ = *in_data;
			}

			else{

				LU = U;
				U  = RU;
				RU = row[i+1][cur_row_out_0];
				L  = M;
				M  = R;
				R  = row[i+1][cur_row_out_1];
				LD = D;
				D  = RD;
				RD = row[i+1][cur_row_out_2];

				int X = (LU+U+RU++L+M+R+LD+D+RD)/9; ////????WRONG


				float tmp = sqrtf((X*X)+(Y*Y));
				out = int(tmp);

				current = out | (out << 8) | (out << 16);
				*out_data++ = *in_data;
				//////////
				int pos_PC;
				int pos_CP;

				in_r = current & 0xFF;
				in_g = (current >> 8) & 0xFF;
				in_b = (current >> 16) & 0xFF;

				pixel_check(on_table,possible_candidates,card_presets,pos_PC,pos_CP,possible_candidates_count, in_r, in_g, in_b);
			}
		}

		cur_row_in = (cur_row_in + 1) % 4;   // points to buffer to write
		cur_row_out_0 = (cur_row_out_0 + 1) % 4;	//points to buffers to read
		cur_row_out_1 = (cur_row_out_1 + 1) % 4;
		cur_row_out_2 = (cur_row_out_2 + 1) % 4;

	}*/
}



