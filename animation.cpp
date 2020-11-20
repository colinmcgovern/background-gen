#include "animation.h"

animation::animation(
     const char* input_file,

     int width,
     int height,
     int channels,
     uint8_t* rgb_image,

     uint num_frames,
     uint delay,

     uint pallete_num,
     double pallete_movement_per_frame,

     double x_movement_per_frame,
     double y_movement_per_frame,

     double x_amp,
     double x_period,
     double x_phase_shift_per_frame,

     double y_amp,
     double y_period,
     double y_phase_shift_per_frame
)
{

        vector<vector<double> > heat_map;

        uint iter = 0;

        for(int i=0;i<width;i++){
            vector<double> row;
            for(int j=0;j<height;j++){
                int sum = 0;
                for(int k=0;k<channels;k++){
                    sum += rgb_image[iter++];
                }
                row.push_back(double(sum)/(256*3));
            }
            heat_map.push_back(row);
        }
        stbi_image_free(rgb_image);

        vector<RGBA> palette;
        switch(pallete_num){

            //Gradient
            case 0:
                for(uint i=0;i<255;i++){
                    palette.push_back({i,i,i,i});
                }
            break;

            //Black and white
            case 1:
                palette.push_back({255,255,255,255});
                palette.push_back({0,0,0,255});
            break;

            //Fire
            case 2:
                palette.push_back({128,17,0,255});
                palette.push_back({182,34,3,255});
                palette.push_back({215,53,2,255});
                palette.push_back({252,100,0,255});
                palette.push_back({255,117,0,255});
                palette.push_back({250,192,0,255});
            break;

            //Melon
            case 3:
                palette.push_back({243,85,136,255});
                palette.push_back({255,187,180,255});
                palette.push_back({113,169,90,255});
                palette.push_back({0,121,68,255});
            break;

            //Matrix
            case 4:
                palette.push_back({01,07,01,255});
                palette.push_back({02,32,02,255});
                palette.push_back({10,66,11,255});
                palette.push_back({18,95,19,255});
                palette.push_back({41,172,43,255});
            break;

            //h i b a s c u s
            case 5:
                palette.push_back({255,188,66,255});
                palette.push_back({216,17,89,255});
                palette.push_back({143,45,86,255});
                palette.push_back({33,131,121,255});
                palette.push_back({115,210,222,255});
            break;

            //blue
            case 6:
                for(uint i=0;i<255;i++){
                    palette.push_back({0,0,i,i});
                }
            break;

            //camo
            case 7:
                palette.push_back({167,123,78,255});
                palette.push_back({186,155,119,255});
                palette.push_back({192,186,169,255});
                palette.push_back({204,202,188,255});
            break;

            //pulse
            case 8:
                for(uint i=0;i<32;i++){
                palette.push_back({i*8,0,0,255});
                }
                for(uint i=0;i<32;i++){
                palette.push_back({(32-i)*8,0,0,255});
                }
            break;

            //highway lights
            case 9:
                for(uint i=0;i<64;i++){
                palette.push_back({0,0,0,255});
                }
                for(uint i=0;i<32;i++){
                palette.push_back({i*4,i*4,0,255});
                }
                for(uint i=0;i<32;i++){
                palette.push_back({(32-i)*4,(32-i)*4,0,255});
                }
            break;

            //blue in and out
            case 10:
                for(uint i=0;i<32;i++){
                palette.push_back({0,0,i*4,255});
                }
                for(uint i=0;i<32;i++){
                palette.push_back({0,0,(32-i)*4,255});
                }
            break;

            //rainbow
            case 11:
                palette.push_back({255,0,0,255});
                palette.push_back({255,255,0,255});
                palette.push_back({0,255,0,255});
                palette.push_back({0,255,255,255});
                palette.push_back({0,0,255,255});
                palette.push_back({255,0,255,255});
            break;

            //thermal
            case 12:
                palette = {convert_to_RGBA("#00000a"),convert_to_RGBA("#000014"),convert_to_RGBA("#00001e"),convert_to_RGBA("#000025"),convert_to_RGBA("#00002a"),convert_to_RGBA("#00002e"),convert_to_RGBA("#000032"),convert_to_RGBA("#000036"),convert_to_RGBA("#00003a"),convert_to_RGBA("#00003e"),convert_to_RGBA("#000042"),convert_to_RGBA("#000046"),convert_to_RGBA("#00004a"),convert_to_RGBA("#00004f"),convert_to_RGBA("#000052"),convert_to_RGBA("#010055"),convert_to_RGBA("#010057"),convert_to_RGBA("#020059"),convert_to_RGBA("#02005c"),convert_to_RGBA("#03005e"),convert_to_RGBA("#040061"),convert_to_RGBA("#040063"),convert_to_RGBA("#050065"),convert_to_RGBA("#060067"),convert_to_RGBA("#070069"),convert_to_RGBA("#08006b"),convert_to_RGBA("#09006e"),convert_to_RGBA("#0a0070"),convert_to_RGBA("#0b0073"),convert_to_RGBA("#0c0074"),convert_to_RGBA("#0d0075"),convert_to_RGBA("#0d0076"),convert_to_RGBA("#0e0077"),convert_to_RGBA("#100078"),convert_to_RGBA("#120079"),convert_to_RGBA("#13007b"),convert_to_RGBA("#15007c"),convert_to_RGBA("#17007d"),convert_to_RGBA("#19007e"),convert_to_RGBA("#1b0080"),convert_to_RGBA("#1c0081"),convert_to_RGBA("#1e0083"),convert_to_RGBA("#200084"),convert_to_RGBA("#220085"),convert_to_RGBA("#240086"),convert_to_RGBA("#260087"),convert_to_RGBA("#280089"),convert_to_RGBA("#2a0089"),convert_to_RGBA("#2c008a"),convert_to_RGBA("#2e008b"),convert_to_RGBA("#30008c"),convert_to_RGBA("#32008d"),convert_to_RGBA("#34008e"),convert_to_RGBA("#36008e"),convert_to_RGBA("#38008f"),convert_to_RGBA("#390090"),convert_to_RGBA("#3b0091"),convert_to_RGBA("#3c0092"),convert_to_RGBA("#3e0093"),convert_to_RGBA("#3f0093"),convert_to_RGBA("#410094"),convert_to_RGBA("#420095"),convert_to_RGBA("#440095"),convert_to_RGBA("#450096"),convert_to_RGBA("#470096"),convert_to_RGBA("#490096"),convert_to_RGBA("#4a0096"),convert_to_RGBA("#4c0097"),convert_to_RGBA("#4e0097"),convert_to_RGBA("#4f0097"),convert_to_RGBA("#510097"),convert_to_RGBA("#520098"),convert_to_RGBA("#540098"),convert_to_RGBA("#560098"),convert_to_RGBA("#580099"),convert_to_RGBA("#5a0099"),convert_to_RGBA("#5c0099"),convert_to_RGBA("#5d009a"),convert_to_RGBA("#5f009a"),convert_to_RGBA("#61009b"),convert_to_RGBA("#63009b"),convert_to_RGBA("#64009b"),convert_to_RGBA("#66009b"),convert_to_RGBA("#68009b"),convert_to_RGBA("#6a009b"),convert_to_RGBA("#6c009c"),convert_to_RGBA("#6d009c"),convert_to_RGBA("#6f009c"),convert_to_RGBA("#70009c"),convert_to_RGBA("#71009d"),convert_to_RGBA("#73009d"),convert_to_RGBA("#75009d"),convert_to_RGBA("#77009d"),convert_to_RGBA("#78009d"),convert_to_RGBA("#7a009d"),convert_to_RGBA("#7c009d"),convert_to_RGBA("#7e009d"),convert_to_RGBA("#7f009d"),convert_to_RGBA("#81009d"),convert_to_RGBA("#83009d"),convert_to_RGBA("#84009d"),convert_to_RGBA("#86009d"),convert_to_RGBA("#87009d"),convert_to_RGBA("#89009d"),convert_to_RGBA("#8a009d"),convert_to_RGBA("#8b009d"),convert_to_RGBA("#8d009d"),convert_to_RGBA("#8f009c"),convert_to_RGBA("#91009c"),convert_to_RGBA("#93009c"),convert_to_RGBA("#95009c"),convert_to_RGBA("#96009b"),convert_to_RGBA("#98009b"),convert_to_RGBA("#99009b"),convert_to_RGBA("#9b009b"),convert_to_RGBA("#9c009b"),convert_to_RGBA("#9d009b"),convert_to_RGBA("#9f009b"),convert_to_RGBA("#a0009b"),convert_to_RGBA("#a2009b"),convert_to_RGBA("#a3009b"),convert_to_RGBA("#a4009b"),convert_to_RGBA("#a6009a"),convert_to_RGBA("#a7009a"),convert_to_RGBA("#a8009a"),convert_to_RGBA("#a90099"),convert_to_RGBA("#aa0099"),convert_to_RGBA("#ab0099"),convert_to_RGBA("#ad0099"),convert_to_RGBA("#ae0198"),convert_to_RGBA("#af0198"),convert_to_RGBA("#b00198"),convert_to_RGBA("#b00198"),convert_to_RGBA("#b10197"),convert_to_RGBA("#b20197"),convert_to_RGBA("#b30196"),convert_to_RGBA("#b40296"),convert_to_RGBA("#b50295"),convert_to_RGBA("#b60295"),convert_to_RGBA("#b70395"),convert_to_RGBA("#b80395"),convert_to_RGBA("#b90495"),convert_to_RGBA("#ba0495"),convert_to_RGBA("#ba0494"),convert_to_RGBA("#bb0593"),convert_to_RGBA("#bc0593"),convert_to_RGBA("#bd0593"),convert_to_RGBA("#be0692"),convert_to_RGBA("#bf0692"),convert_to_RGBA("#bf0692"),convert_to_RGBA("#c00791"),convert_to_RGBA("#c00791"),convert_to_RGBA("#c10890"),convert_to_RGBA("#c10990"),convert_to_RGBA("#c20a8f"),convert_to_RGBA("#c30a8e"),convert_to_RGBA("#c30b8e"),convert_to_RGBA("#c40c8d"),convert_to_RGBA("#c50c8c"),convert_to_RGBA("#c60d8b"),convert_to_RGBA("#c60e8a"),convert_to_RGBA("#c70f89"),convert_to_RGBA("#c81088"),convert_to_RGBA("#c91187"),convert_to_RGBA("#ca1286"),convert_to_RGBA("#ca1385"),convert_to_RGBA("#cb1385"),convert_to_RGBA("#cb1484"),convert_to_RGBA("#cc1582"),convert_to_RGBA("#cd1681"),convert_to_RGBA("#ce1780"),convert_to_RGBA("#ce187e"),convert_to_RGBA("#cf187c"),convert_to_RGBA("#cf197b"),convert_to_RGBA("#d01a79"),convert_to_RGBA("#d11b78"),convert_to_RGBA("#d11c76"),convert_to_RGBA("#d21c75"),convert_to_RGBA("#d21d74"),convert_to_RGBA("#d31e72"),convert_to_RGBA("#d32071"),convert_to_RGBA("#d4216f"),convert_to_RGBA("#d4226e"),convert_to_RGBA("#d5236b"),convert_to_RGBA("#d52469"),convert_to_RGBA("#d62567"),convert_to_RGBA("#d72665"),convert_to_RGBA("#d82764"),convert_to_RGBA("#d82862"),convert_to_RGBA("#d92a60"),convert_to_RGBA("#da2b5e"),convert_to_RGBA("#da2c5c"),convert_to_RGBA("#db2e5a"),convert_to_RGBA("#db2f57"),convert_to_RGBA("#dc2f54"),convert_to_RGBA("#dd3051"),convert_to_RGBA("#dd314e"),convert_to_RGBA("#de324a"),convert_to_RGBA("#de3347"),convert_to_RGBA("#df3444"),convert_to_RGBA("#df3541"),convert_to_RGBA("#df363d"),convert_to_RGBA("#e0373a"),convert_to_RGBA("#e03837"),convert_to_RGBA("#e03933"),convert_to_RGBA("#e13a30"),convert_to_RGBA("#e23b2d"),convert_to_RGBA("#e23c2a"),convert_to_RGBA("#e33d26"),convert_to_RGBA("#e33e23"),convert_to_RGBA("#e43f20"),convert_to_RGBA("#e4411d"),convert_to_RGBA("#e4421c"),convert_to_RGBA("#e5431b"),convert_to_RGBA("#e54419"),convert_to_RGBA("#e54518"),convert_to_RGBA("#e64616"),convert_to_RGBA("#e74715"),convert_to_RGBA("#e74814"),convert_to_RGBA("#e74913"),convert_to_RGBA("#e84a12"),convert_to_RGBA("#e84c10"),convert_to_RGBA("#e84c0f"),convert_to_RGBA("#e94d0e"),convert_to_RGBA("#e94d0d"),convert_to_RGBA("#ea4e0c"),convert_to_RGBA("#ea4f0c"),convert_to_RGBA("#eb500b"),convert_to_RGBA("#eb510a"),convert_to_RGBA("#eb520a"),convert_to_RGBA("#eb5309"),convert_to_RGBA("#ec5409"),convert_to_RGBA("#ec5608"),convert_to_RGBA("#ec5708"),convert_to_RGBA("#ec5808"),convert_to_RGBA("#ed5907"),convert_to_RGBA("#ed5a07"),convert_to_RGBA("#ed5b06"),convert_to_RGBA("#ee5c06"),convert_to_RGBA("#ee5c05"),convert_to_RGBA("#ee5d05"),convert_to_RGBA("#ee5e05"),convert_to_RGBA("#ef5f04"),convert_to_RGBA("#ef6004"),convert_to_RGBA("#ef6104"),convert_to_RGBA("#ef6204"),convert_to_RGBA("#f06303"),convert_to_RGBA("#f06403"),convert_to_RGBA("#f06503"),convert_to_RGBA("#f16603"),convert_to_RGBA("#f16603"),convert_to_RGBA("#f16703"),convert_to_RGBA("#f16803"),convert_to_RGBA("#f16902"),convert_to_RGBA("#f16a02"),convert_to_RGBA("#f16b02"),convert_to_RGBA("#f16b02"),convert_to_RGBA("#f26c01"),convert_to_RGBA("#f26d01"),convert_to_RGBA("#f26e01"),convert_to_RGBA("#f36f01"),convert_to_RGBA("#f37001"),convert_to_RGBA("#f37101"),convert_to_RGBA("#f37201"),convert_to_RGBA("#f47300"),convert_to_RGBA("#f47400"),convert_to_RGBA("#f47500"),convert_to_RGBA("#f47600"),convert_to_RGBA("#f47700"),convert_to_RGBA("#f47800"),convert_to_RGBA("#f47a00"),convert_to_RGBA("#f57b00"),convert_to_RGBA("#f57c00"),convert_to_RGBA("#f57e00"),convert_to_RGBA("#f57f00"),convert_to_RGBA("#f68000"),convert_to_RGBA("#f68100"),convert_to_RGBA("#f68200"),convert_to_RGBA("#f78300"),convert_to_RGBA("#f78400"),convert_to_RGBA("#f78500"),convert_to_RGBA("#f78600"),convert_to_RGBA("#f88700"),convert_to_RGBA("#f88800"),convert_to_RGBA("#f88800"),convert_to_RGBA("#f88900"),convert_to_RGBA("#f88a00"),convert_to_RGBA("#f88b00"),convert_to_RGBA("#f88c00"),convert_to_RGBA("#f98d00"),convert_to_RGBA("#f98d00"),convert_to_RGBA("#f98e00"),convert_to_RGBA("#f98f00"),convert_to_RGBA("#f99000"),convert_to_RGBA("#f99100"),convert_to_RGBA("#f99200"),convert_to_RGBA("#f99300"),convert_to_RGBA("#fa9400"),convert_to_RGBA("#fa9500"),convert_to_RGBA("#fa9600"),convert_to_RGBA("#fb9800"),convert_to_RGBA("#fb9900"),convert_to_RGBA("#fb9a00"),convert_to_RGBA("#fb9c00"),convert_to_RGBA("#fc9d00"),convert_to_RGBA("#fc9f00"),convert_to_RGBA("#fca000"),convert_to_RGBA("#fca100"),convert_to_RGBA("#fda200"),convert_to_RGBA("#fda300"),convert_to_RGBA("#fda400"),convert_to_RGBA("#fda600"),convert_to_RGBA("#fda700"),convert_to_RGBA("#fda800"),convert_to_RGBA("#fdaa00"),convert_to_RGBA("#fdab00"),convert_to_RGBA("#fdac00"),convert_to_RGBA("#fdad00"),convert_to_RGBA("#fdae00"),convert_to_RGBA("#feaf00"),convert_to_RGBA("#feb000"),convert_to_RGBA("#feb100"),convert_to_RGBA("#feb200"),convert_to_RGBA("#feb300"),convert_to_RGBA("#feb400"),convert_to_RGBA("#feb500"),convert_to_RGBA("#feb600"),convert_to_RGBA("#feb800"),convert_to_RGBA("#feb900"),convert_to_RGBA("#feb900"),convert_to_RGBA("#feba00"),convert_to_RGBA("#febb00"),convert_to_RGBA("#febc00"),convert_to_RGBA("#febd00"),convert_to_RGBA("#febe00"),convert_to_RGBA("#fec000"),convert_to_RGBA("#fec100"),convert_to_RGBA("#fec200"),convert_to_RGBA("#fec300"),convert_to_RGBA("#fec400"),convert_to_RGBA("#fec500"),convert_to_RGBA("#fec600"),convert_to_RGBA("#fec700"),convert_to_RGBA("#fec800"),convert_to_RGBA("#fec901"),convert_to_RGBA("#feca01"),convert_to_RGBA("#feca01"),convert_to_RGBA("#fecb01"),convert_to_RGBA("#fecc02"),convert_to_RGBA("#fecd02"),convert_to_RGBA("#fece03"),convert_to_RGBA("#fecf04"),convert_to_RGBA("#fecf04"),convert_to_RGBA("#fed005"),convert_to_RGBA("#fed106"),convert_to_RGBA("#fed308"),convert_to_RGBA("#fed409"),convert_to_RGBA("#fed50a"),convert_to_RGBA("#fed60a"),convert_to_RGBA("#fed70b"),convert_to_RGBA("#fed80c"),convert_to_RGBA("#fed90d"),convert_to_RGBA("#ffda0e"),convert_to_RGBA("#ffda0e"),convert_to_RGBA("#ffdb10"),convert_to_RGBA("#ffdc12"),convert_to_RGBA("#ffdc14"),convert_to_RGBA("#ffdd16"),convert_to_RGBA("#ffde19"),convert_to_RGBA("#ffde1b"),convert_to_RGBA("#ffdf1e"),convert_to_RGBA("#ffe020"),convert_to_RGBA("#ffe122"),convert_to_RGBA("#ffe224"),convert_to_RGBA("#ffe226"),convert_to_RGBA("#ffe328"),convert_to_RGBA("#ffe42b"),convert_to_RGBA("#ffe42e"),convert_to_RGBA("#ffe531"),convert_to_RGBA("#ffe635"),convert_to_RGBA("#ffe638"),convert_to_RGBA("#ffe73c"),convert_to_RGBA("#ffe83f"),convert_to_RGBA("#ffe943"),convert_to_RGBA("#ffea46"),convert_to_RGBA("#ffeb49"),convert_to_RGBA("#ffeb4d"),convert_to_RGBA("#ffec50"),convert_to_RGBA("#ffed54"),convert_to_RGBA("#ffee57"),convert_to_RGBA("#ffee5b"),convert_to_RGBA("#ffee5f"),convert_to_RGBA("#ffef63"),convert_to_RGBA("#ffef67"),convert_to_RGBA("#fff06a"),convert_to_RGBA("#fff06e"),convert_to_RGBA("#fff172"),convert_to_RGBA("#fff177"),convert_to_RGBA("#fff17b"),convert_to_RGBA("#fff280"),convert_to_RGBA("#fff285"),convert_to_RGBA("#fff28a"),convert_to_RGBA("#fff38e"),convert_to_RGBA("#fff492"),convert_to_RGBA("#fff496"),convert_to_RGBA("#fff49a"),convert_to_RGBA("#fff59e"),convert_to_RGBA("#fff5a2"),convert_to_RGBA("#fff5a6"),convert_to_RGBA("#fff6aa"),convert_to_RGBA("#fff6af"),convert_to_RGBA("#fff7b3"),convert_to_RGBA("#fff7b6"),convert_to_RGBA("#fff8ba"),convert_to_RGBA("#fff8bd"),convert_to_RGBA("#fff8c1"),convert_to_RGBA("#fff8c4"),convert_to_RGBA("#fff9c7"),convert_to_RGBA("#fff9ca"),convert_to_RGBA("#fff9cd"),convert_to_RGBA("#fffad1"),convert_to_RGBA("#fffad4"),convert_to_RGBA("#fffbd8"),convert_to_RGBA("#fffcdb"),convert_to_RGBA("#fffcdf"),convert_to_RGBA("#fffde2"),convert_to_RGBA("#fffde5"),convert_to_RGBA("#fffde8"),convert_to_RGBA("#fffeeb"),convert_to_RGBA("#fffeee"),convert_to_RGBA("#fffef1"),convert_to_RGBA("#fffef4"),convert_to_RGBA("#fffff6")};
            break;
        }

        double palette_offset = 0;

        double x_offset = 0;
        double y_offset = 0;

        double x_phase_shift = 0;
        double y_phase_shift = 0;

        auto fileName = "output.gif";
        GifWriter g;
        GifBegin(&g, fileName, heat_map.size(),
                    heat_map[0].size(), delay);
        for(uint i=0;i<num_frames;i++){

            vector<uint8_t> image;

            palette_offset += pallete_movement_per_frame;

            x_phase_shift += x_phase_shift_per_frame;
            y_phase_shift += y_phase_shift_per_frame;

            x_offset += x_movement_per_frame;
            y_offset += y_movement_per_frame;

            frame(image,heat_map,palette,int(palette_offset),
                int(x_offset),int(y_offset),
                x_amp,x_period,x_phase_shift,
                y_amp,y_period,y_phase_shift);

            GifWriteFrame(&g, image.data(), heat_map.size(),
                    heat_map[0].size(), delay);
        }
        GifEnd(&g);

        qDebug() << "done" << endl;
}
