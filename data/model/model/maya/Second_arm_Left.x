xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 58;
 -1.28726;-2.61202;0.27612;,
 -1.48804;0.22484;3.74278;,
 -3.00444;-1.59198;2.06576;,
 -3.09778;-2.23546;0.20378;,
 -0.38483;2.69215;3.25502;,
 -1.86279;1.77586;2.10815;,
 -0.31919;3.81873;-0.01747;,
 -2.15205;2.81303;0.02855;,
 -1.28541;-1.36662;-3.46991;,
 -3.26972;-1.16543;-1.61327;,
 -0.27941;2.70133;-2.55176;,
 -1.77871;1.88574;-1.64538;,
 -20.60696;-0.30549;1.59735;,
 -20.46205;-1.45586;-0.05150;,
 -20.46208;-0.08077;-1.32169;,
 -20.32386;2.11555;-1.31123;,
 -20.26455;3.14116;-0.04030;,
 -20.41083;2.03366;1.58369;,
 -2.49480;0.37048;-1.60785;,
 -1.77075;1.88557;-1.64504;,
 -1.77871;1.88574;-1.64538;,
 -1.77075;1.88557;-1.64504;,
 -1.94681;2.35447;-0.79850;,
 -1.77871;1.88574;-1.64538;,
 2.98047;0.25491;-0.45267;,
 3.06048;0.42606;-0.16351;,
 2.79213;-0.26150;-0.60150;,
 2.77178;-0.43373;-0.11138;,
 2.96805;0.24081;0.24273;,
 2.79265;-0.07821;0.29606;,
 -1.94681;2.35447;-0.79850;,
 -2.49480;0.37048;-1.60785;,
 -0.08374;-2.51000;0.37128;,
 0.22606;-1.36333;2.85200;,
 0.04973;-1.69119;-3.03033;,
 1.65436;2.27671;-2.25065;,
 2.16213;3.02015;0.00927;,
 1.58552;2.20178;3.22517;,
 1.40853;-1.84296;0.23671;,
 1.48647;-0.94792;2.39798;,
 1.49923;-1.08263;-2.11318;,
 2.41525;1.35281;-1.49942;,
 2.85626;2.10156;0.00804;,
 2.27210;1.12784;2.77493;,
 2.41044;-0.89490;0.02231;,
 2.40394;-0.41262;1.33702;,
 2.46039;-0.49194;-1.10497;,
 2.93272;0.80310;-0.73172;,
 2.76499;0.57071;1.44094;,
 3.03585;1.15138;-0.10681;,
 -3.26972;-1.16543;-1.61327;,
 -1.77871;1.88574;-1.64538;,
 -1.77871;1.88574;-1.64538;,
 -2.15205;2.81303;0.02855;,
 -2.15205;2.81303;0.02855;,
 -3.09778;-2.23546;0.20378;,
 -1.86279;1.77586;2.10815;,
 -3.00444;-1.59198;2.06576;;
 
 58;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;8,0,3,9;,
 4;6,10,11,7;,
 4;10,8,9,11;,
 3;5,7,2;,
 3;2,7,3;,
 3;7,11,3;,
 3;11,9,3;,
 4;3,2,12,13;,
 4;9,3,13,14;,
 4;11,9,14,15;,
 4;7,11,15,16;,
 4;5,7,16,17;,
 4;2,5,17,12;,
 3;17,16,12;,
 3;12,16,13;,
 3;16,15,13;,
 3;15,14,13;,
 3;18,19,20;,
 3;21,22,23;,
 3;24,25,26;,
 3;26,25,27;,
 3;25,28,27;,
 3;28,29,27;,
 3;30,31,23;,
 4;1,0,32,33;,
 4;0,8,34,32;,
 4;8,10,35,34;,
 4;10,6,36,35;,
 4;6,4,37,36;,
 4;4,1,33,37;,
 4;33,32,38,39;,
 4;32,34,40,38;,
 4;34,35,41,40;,
 4;35,36,42,41;,
 4;36,37,43,42;,
 4;37,33,39,43;,
 4;39,38,44,45;,
 4;38,40,46,44;,
 4;40,41,47,46;,
 4;43,39,45,48;,
 4;45,44,27,29;,
 4;44,46,26,27;,
 4;46,47,24,26;,
 4;48,45,29,28;,
 4;49,48,28,25;,
 4;47,49,25,24;,
 4;41,42,49,47;,
 4;42,43,48,49;,
 4;50,51,19,18;,
 4;52,53,22,21;,
 3;31,30,50;,
 3;30,54,50;,
 3;50,54,55;,
 3;54,56,55;,
 3;56,57,55;;
 
 MeshMaterialList {
  1;
  58;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  77;
  -0.030329;-0.879984;0.474034;,
  -0.107268;-0.993993;-0.021726;,
  0.009684;-0.532813;-0.846178;,
  -0.587285;0.680283;-0.438534;,
  -0.872861;0.487333;-0.024929;,
  -0.617850;0.638014;0.459564;,
  -0.124329;-0.987827;0.093487;,
  -0.085289;-0.859154;0.504559;,
  -0.679775;0.581180;0.447366;,
  -0.153619;0.987204;-0.042761;,
  -0.140132;-0.930610;-0.338125;,
  -0.331427;0.743897;-0.580322;,
  -0.061681;-0.890212;0.451352;,
  -0.033854;-0.430704;-0.901858;,
  -0.004518;0.470791;-0.882233;,
  0.009333;0.829501;-0.558427;,
  -0.011994;0.497907;0.867147;,
  0.179847;-0.979039;0.095595;,
  0.218118;-0.727186;0.650864;,
  0.212218;-0.942398;-0.258553;,
  0.551372;0.411928;-0.725469;,
  0.604345;0.795419;-0.045555;,
  0.568393;0.754947;0.327083;,
  0.570831;-0.820229;0.037096;,
  0.611963;-0.584953;0.532288;,
  0.673516;-0.625698;-0.393545;,
  0.803624;0.155465;-0.574472;,
  0.920388;0.387325;-0.053530;,
  0.922557;0.111477;0.369407;,
  0.755374;-0.654514;0.031983;,
  0.813799;-0.487000;0.317115;,
  0.801684;-0.438905;-0.405789;,
  0.900511;-0.000249;-0.434834;,
  0.992054;0.121379;-0.033091;,
  0.941512;-0.151645;0.300931;,
  0.889929;-0.448279;0.084101;,
  0.856556;-0.431354;0.283278;,
  0.901482;-0.376865;-0.212846;,
  0.949705;-0.168524;-0.263932;,
  0.980671;-0.194826;-0.018084;,
  0.934480;-0.285387;0.212844;,
  -0.042310;0.044715;0.998103;,
  0.587542;-0.630944;0.506660;,
  -0.778974;0.355893;0.516275;,
  -0.918516;0.290319;0.268408;,
  -0.205522;0.244493;-0.947620;,
  -0.622699;0.308859;-0.718924;,
  -0.963738;0.266824;0.003813;,
  -0.847939;0.417792;0.326266;,
  -0.043720;-0.778544;-0.626066;,
  0.174976;-0.097415;-0.979742;,
  0.018808;0.854162;-0.519667;,
  0.006329;0.862252;0.506440;,
  -0.027161;0.002517;0.999628;,
  -0.995980;0.083315;-0.032893;,
  -0.997341;0.056463;-0.046083;,
  -0.995173;0.063182;-0.075098;,
  -0.998693;0.049532;-0.012574;,
  -0.997821;0.052640;0.039783;,
  -0.995768;0.062346;0.067523;,
  -0.039225;-0.877533;0.477908;,
  0.839537;-0.387315;0.381006;,
  0.818838;-0.395779;0.415768;,
  0.259025;0.123776;-0.957907;,
  0.444132;0.032401;-0.895375;,
  0.262269;0.916246;0.302833;,
  0.178804;-0.020443;0.983672;,
  0.268115;-0.142163;0.952840;,
  0.608577;-0.060482;-0.791186;,
  0.351060;-0.260508;0.899384;,
  0.358994;-0.192275;-0.913320;,
  0.028241;0.876944;-0.479762;,
  0.905991;-0.350430;0.237444;,
  0.961752;-0.273918;-0.000964;,
  0.964428;-0.234216;-0.122562;,
  0.962808;-0.257327;-0.082356;,
  0.944904;-0.321110;0.063605;;
  58;
  4;6,7,0,1;,
  4;43,8,5,44;,
  4;8,9,4,5;,
  4;10,6,1,2;,
  4;9,11,3,4;,
  4;11,45,46,3;,
  3;5,4,44;,
  3;44,4,47;,
  3;4,3,47;,
  3;3,48,47;,
  4;1,0,12,12;,
  4;2,1,49,13;,
  4;50,2,13,14;,
  4;51,3,14,15;,
  4;5,52,52,16;,
  4;53,53,16,53;,
  3;54,55,56;,
  3;56,55,57;,
  3;55,58,57;,
  3;58,59,57;,
  3;41,41,41;,
  3;60,42,60;,
  3;38,39,37;,
  3;37,39,35;,
  3;39,40,35;,
  3;40,36,35;,
  3;61,62,61;,
  4;7,6,17,18;,
  4;6,10,19,17;,
  4;45,63,20,64;,
  4;11,9,21,20;,
  4;9,65,22,21;,
  4;66,66,66,67;,
  4;18,17,23,24;,
  4;17,19,25,23;,
  4;64,20,26,68;,
  4;20,21,27,26;,
  4;21,22,28,27;,
  4;67,18,24,69;,
  4;24,23,29,30;,
  4;23,25,31,29;,
  4;25,26,32,31;,
  4;28,24,30,34;,
  4;30,29,35,36;,
  4;29,31,37,35;,
  4;31,32,38,37;,
  4;34,30,36,40;,
  4;33,34,40,39;,
  4;32,33,39,38;,
  4;26,27,33,32;,
  4;27,28,34,33;,
  4;2,50,70,70;,
  4;3,51,71,71;,
  3;62,42,72;,
  3;42,73,72;,
  3;72,73,74;,
  3;73,75,74;,
  3;75,76,74;;
 }
 MeshTextureCoords {
  58;
  0.750000;0.125000;,
  0.875000;0.125000;,
  0.875000;0.125000;,
  0.750000;0.125000;,
  0.875000;0.250000;,
  0.875000;0.250000;,
  0.750000;0.250000;,
  0.750000;0.250000;,
  0.625000;0.125000;,
  0.625000;0.125000;,
  0.625000;0.250000;,
  0.625000;0.250000;,
  0.875000;0.125000;,
  0.750000;0.125000;,
  0.625000;0.125000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  0.500000;1.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.500000;0.750000;,
  0.000000;1.000000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.875000;0.250000;,
  0.875000;0.125000;,
  0.000000;0.750000;,
  0.000000;1.000000;,
  0.750000;0.125000;,
  0.875000;0.125000;,
  0.625000;0.125000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  0.750000;0.125000;,
  0.875000;0.125000;,
  0.625000;0.125000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  0.750000;0.125000;,
  0.875000;0.125000;,
  0.625000;0.125000;,
  0.625000;0.250000;,
  0.875000;0.250000;,
  0.734375;0.250000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.500000;;
 }
}
