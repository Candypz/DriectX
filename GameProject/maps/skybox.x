xof 0303txt 0032

// DirectX - from MilkShape3D

Mesh skyboxMesh
{
    24;
    24.000000;27.950001;-24.050001;,
    -24.000000;-20.050001;-24.050001;,
    -24.000000;27.950001;-24.050001;,
    24.000000;-20.050001;-24.050001;,
    -24.000000;27.950001;-24.050001;,
    -24.000000;-20.050001;23.950001;,
    -24.000000;27.950001;23.950001;,
    -24.000000;-20.050001;-24.050001;,
    24.000000;27.950001;23.950001;,
    24.000000;-20.050001;-24.050001;,
    24.000000;27.950001;-24.050001;,
    24.000000;-20.050001;23.950001;,
    24.000000;27.950001;23.950001;,
    -24.000000;27.950001;-24.050001;,
    -24.000000;27.950001;23.950001;,
    24.000000;27.950001;-24.050001;,
    -24.000000;27.950001;23.950001;,
    24.000000;-20.050001;23.950001;,
    24.000000;27.950001;23.950001;,
    -24.000000;-20.050001;23.950001;,
    24.000000;-20.050001;-24.050001;,
    -24.000000;-20.050001;23.950001;,
    -24.000000;-20.050001;-24.050001;,
    24.000000;-20.050001;23.950001;;
    12;
    3;0,2,1;,
    3;0,1,3;,
    3;4,6,5;,
    3;4,5,7;,
    3;8,10,9;,
    3;8,9,11;,
    3;12,14,13;,
    3;12,13,15;,
    3;16,18,17;,
    3;16,17,19;,
    3;20,22,21;,
    3;20,21,23;;

    MeshNormals
    {
        6;
        0.000000;0.000000;1.000000;,
        1.000000;0.000000;0.000000;,
        -1.000000;0.000000;0.000000;,
        0.000000;-1.000000;0.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;1.000000;0.000000;;
        12;
        3;0,0,0;,
        3;0,0,0;,
        3;1,1,1;,
        3;1,1,1;,
        3;2,2,2;,
        3;2,2,2;,
        3;3,3,3;,
        3;3,3,3;,
        3;4,4,4;,
        3;4,4,4;,
        3;5,5,5;,
        3;5,5,5;;
    }

    MeshTextureCoords
    {
        24;
        1.000000;0.000000;,
        0.000000;1.000000;,
        0.000000;0.000000;,
        1.000000;1.000000;,
        1.000000;0.000000;,
        0.000000;1.000000;,
        0.000000;0.000000;,
        1.000000;1.000000;,
        1.000000;0.000000;,
        0.000000;1.000000;,
        0.000000;0.000000;,
        1.000000;1.000000;,
        1.000000;0.000000;,
        0.000000;1.000000;,
        0.000000;0.000000;,
        1.000000;1.000000;,
        1.000000;0.000000;,
        0.000000;1.000000;,
        0.000000;0.000000;,
        1.000000;1.000000;,
        1.000000;0.000000;,
        0.000000;1.000000;,
        0.000000;0.000000;,
        1.000000;1.000000;;
    }

    MeshMaterialList
    {
        6;
        12;
        0,
        0,
        1,
        1,
        2,
        2,
        3,
        3,
        4,
        4,
        5,
        5;

        Material Material01
        {
            0.800000;0.800000;0.800000;1.000000;;
            0.000000;
            0.000000;0.000000;0.000000;;
            0.000000;0.000000;0.000000;;

            TextureFileName
            {
                "maps/skyDay_front.jpg";
            }
        }

        Material Material02
        {
            0.800000;0.800000;0.800000;1.000000;;
            0.000000;
            0.000000;0.000000;0.000000;;
            0.000000;0.000000;0.000000;;

            TextureFileName
            {
                "maps/skyDay_left.jpg";
            }
        }

        Material Material03
        {
            0.800000;0.800000;0.800000;1.000000;;
            0.000000;
            0.000000;0.000000;0.000000;;
            0.000000;0.000000;0.000000;;

            TextureFileName
            {
                "maps/skyDay_right.jpg";
            }
        }

        Material Material05
        {
            0.800000;0.800000;0.800000;1.000000;;
            0.000000;
            0.000000;0.000000;0.000000;;
            0.000000;0.000000;0.000000;;

            TextureFileName
            {
                "maps/skyDay_top.jpg";
            }
        }

        Material Material04
        {
            0.800000;0.800000;0.800000;1.000000;;
            0.000000;
            0.000000;0.000000;0.000000;;
            0.000000;0.000000;0.000000;;

            TextureFileName
            {
                "maps/skyDay_back.jpg";
            }
        }

        Material Material06
        {
            0.800000;0.800000;0.800000;1.000000;;
            0.000000;
            0.000000;0.000000;0.000000;;
            0.000000;0.000000;0.000000;;

            TextureFileName
            {
                "maps/sky_dust_bottom.jpg";
            }
        }
    }
}
