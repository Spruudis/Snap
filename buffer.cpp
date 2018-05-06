






struct my_buffer{

  void initialise(){


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




  unsigned char get_average(){
    if(output_column== w){
      output_column = 0; // resets column counter
      output_row++;

      row_in = (row_in + 1) % 4;   // points to buffer to write
  		row_out_0 = (row_out_0 + 1) % 4;	//points to buffers to read
  		row_out_1 = (row_out_1 + 1) % 4;
  		row_out_2 = (row_out_2 + 1) % 4;
    }

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
        L = M
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
        L = M
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
        L = M
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
        L = M
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
    if(output column == 0){
      return L;
    }
    else if(output column == w-1){
      return R;
    }
    else return M;
  }





  static uchar buff[1920][4];


  uchar LU;
	uchar U;
	uchar RU;
	uchar L;
	uchar M;
	uchar R;
	uchar LD;
	uchar D;
	uchar RD;

  uchar row_in;   // points to buffer to write
  uchar row_out_0;	//points to buffers to read
  uchar row_out_1;
  uchar row_out_2;

  int output_row, output_column;
};
