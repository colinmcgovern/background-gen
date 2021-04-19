#include "eb_frame.h"

RGBA eb_frame::convert_to_RGBA(string s){
    
    RGBA output;
        
    output[0] = stoul(s.substr(1,2), nullptr, 16);
    output[1] = stoul(s.substr(3,2), nullptr, 16);
    output[2] = stoul(s.substr(5,2), nullptr, 16);
    output[3] = 0;
    
    return output;
}

vector<RGBA> eb_frame::convert_to_RGBA(vector<string> s){
    
    vector<RGBA> output;
        
    for(auto v : s){
    	output.push_back(convert_to_RGBA(v));
    }
    
    return output;
}

vector<vector<RGBA>> eb_frame::simplify(uint8_t* input, int width,
									 int height){

	vector<vector<RGBA>> output;

	uint index = 0;

	for(size_t i=0;i<width;i++){

		vector<RGBA> new_row;

		for(size_t j=0;j<height;j++){

			RGBA new_val;

			for(size_t k=0;k<4;k++){
				//cout << index << ": " << i << " " << j << " " << k << endl; //del
				new_val[k] = input[index++];

			}

			new_row.push_back(new_val);

		}

		output.push_back(new_row);
	}

	return output;
}

uint8_t* eb_frame::desimplify(vector<vector<RGBA>> input){

	if(input.size()==0 || input[0].size()==0){
		uint8_t* output;
		return output;
	}

	uint8_t* output = (uint8_t*)malloc(	 sizeof(uint8_t) *
										 input.size() *
								  		 input[0].size() * 
								  		 input[1].size() );
	for(size_t i=0;i<input.size();i++){
		for(size_t j=0;j<input[i].size();j++){
			for(size_t k=0;k<input[i][j].size();k++){
				int index = k + i * 4 + j * 4 * input.size();
				output[index] = input[i][j][k];
				//cout << "desimplify: " << index << ": " << i << " " << j << " " << k << endl; //del
			}
		}
	}
	return output;
}

void eb_frame::apply_palette(){

	vector<vector<RGBA>> my_image = simplify(image,width,height);

	cout << "my_image " << width << " " << height << endl; //del

	for(int i=0;i<my_image.size();i++){
    	vector<double> row;
    	for(int j=0;j<my_image[i].size();j++){

    		cout << i << " " << j << endl; //del
    		
    		double sum = 0;
    		for(int k=0;k<3;k++){
    			//cout << i << " " << j << " " << k << endl; //del
	    		sum += my_image[i][j][k];
	    	}
	    	sum /= 256*3;

	    	int starting_palette_index = sum * (palette.size()-1);
	    	int animation_offset = 	double(palette.size()) * 
	    							palette_cycles_per_round *
	    							round_progress;

	    	//cout << animation_offset << ": " << double(palette.size()-1) << " " << palette_cycles_per_round << " " << round_progress << endl; //del
			
			int palette_index = starting_palette_index + animation_offset
								+ palette_offset;

			cout << starting_palette_index << " " << animation_offset << " " << palette_index << " " << palette_cycles_per_round << " " << round_progress << endl; //del

	    	my_image[i][j] = palette[palette_index%(palette.size())];
    	}
    }

	image = desimplify(my_image);
}

void eb_frame::apply_hor_osc(){

}

void eb_frame::apply_vert_osc(){

}

void eb_frame::apply_translation(){

}

eb_frame::eb_frame(	uint8_t* image,
					int width,
					int height,
					double round_progress,
					vector<RGBA> palette,
					int palette_offset,
					double palette_cycles_per_round,
					int x_offset,
					double x_cycles_per_round,
					int y_offset,
					double y_cycles_per_round,
					double x_amp_min,
					double x_amp_max,
					double x_amp_cycles_per_round,
					double x_period_min,
					double x_period_max,
					double x_period_cycles_per_round,
					double x_phase_offset,
					double x_phase_cycles_per_round,
					double y_amp_min,
					double y_amp_max,
					double y_amp_cycles_per_round,
					double y_period_min,
					double y_period_max,
					double y_period_cycles_per_round,
					double y_phase_offset,
					double y_phase_cycles_per_round
			) : 
					image(image),
					width(width),
					height(height),
					round_progress(round_progress),
					palette(palette),
					palette_offset(palette_offset),
					palette_cycles_per_round(palette_cycles_per_round),
					x_offset(x_offset),
					x_cycles_per_round(x_cycles_per_round),
					y_offset(),
					y_cycles_per_round(y_offset),
					x_amp_min(x_amp_min),
					x_amp_max(x_amp_max),
					x_amp_cycles_per_round(x_amp_cycles_per_round),
					x_period_min(x_period_min),
					x_period_max(x_period_max),
					x_period_cycles_per_round(x_period_cycles_per_round),
					x_phase_offset(x_phase_offset),
					x_phase_cycles_per_round(x_phase_cycles_per_round),
					y_amp_min(y_amp_min),
					y_amp_max(y_amp_max),
					y_amp_cycles_per_round(y_amp_cycles_per_round),
					y_period_min(y_period_min),
					y_period_max(y_period_max),
					y_period_cycles_per_round(y_period_cycles_per_round),
					y_phase_offset(y_phase_offset),
					y_phase_cycles_per_round(y_phase_cycles_per_round)



	{

	//TESTS
	if(image!=desimplify(simplify(image,width,height))){

		//del
		// uint8_t* image_before = image;
		// uint8_t* image_after = desimplify(simplify(image,width,height));

		// uint index = 0;

		// for(uint i=0;i<width;i++){
		// 	for(uint j=0;j<width;j++){
		// 		for(uint k=0;k<4;k++){
		// 			cout << i << " " << j << " " << k << ":" << to_string(image_before[index]) << " " << to_string(image_after[index]) << endl; //del
		// 		}
		// 	}
		// }
		//del

		cout << "desimplify simplify fail" << endl;
		//exit(0);
	}
	//TESTS	

	//cout << "a" << endl; //del

	apply_palette();
	//cout << "b" << endl; //del
	apply_hor_osc();
	apply_vert_osc();
	apply_translation();
}

// vector<string> split(const string& str(), const string& delim){
//     vector<string> tokens;
//     size_t prev = 0, pos = 0;
//     do
//     {
//         pos = str.find(delim, prev);
//         if (pos == string::npos) pos = str.length();
//         string token = str.substr(prev, pos-prev);
//         if (!token.empty()) tokens.push_back(token);
//         prev = pos + delim.length();
//     }
//     while (pos < str.length() && prev < str.length());
//     return tokens;
// }

