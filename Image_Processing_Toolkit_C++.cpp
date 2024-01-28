#include <iostream>
#include <fstream>
#include <vector>
#include<string>
#include <math.h>
using namespace std;

void B_Sort(int a[], int s) {
    for (int i = 0; i < s - 1; i++) {
        for (int j = 0; j < s - 2 - i; j++) {
            if (a[j] < a[j + 1]) {
                int T = a[j];
                a[j] = a[j + 1];
                a[j + 1] = T;
            }
        }
    }
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


struct PGMImage {
    int H, W, MaxGray;
    string Comment;
    string MagicNo;
    vector<vector<int>> ImageData;

    PGMImage() {
        cout << "Called Automatically" << endl;
        H = W = MaxGray = -1;
    }

    void Create(vector<vector<int>>& Img, int H, int W) {
        Img = vector<vector<int>>(H);
        for (int i = 0; i < H; i++)
            Img[i] = vector<int>(W);
        return;
    }

    bool ReserveMemory() {
        // Code to create memory storage for the image
        // This code assumes that H and W have been set earlier
        cout << W << " " << H << endl;
        if (H < 0 || W < 0)
            return false;
        Create(ImageData, H, W);
        return true;
    }

    bool LoadImage(char FileName[]) {
        // Code to open File, Reserve Memory and load image data in memory
        ifstream FIN(FileName);
        if (!FIN.is_open())
            return false;

        getline(FIN, MagicNo);
        getline(FIN, Comment); // reading the comment. PROBLEM HERE THE COMMENTIS OPTIONAL

        FIN >> W >> H >> MaxGray;
        if (!ReserveMemory()) {
            FIN.close();
            return false;
        }

        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++) {
                FIN >> ImageData[r][c];
            }
        }

        FIN.close();
        return true;
    }

    bool SaveImage(char FileName[]) {
        // Code to open File, Reserve Memory and load image data in memory
        if (H < 0)
            return false;

        ofstream FOUT(FileName);
        // Your Code to check if the file has been created for output
        // If File not created then return false

        FOUT << "P2" << endl
            << "# Created By MB For BDS-1A and BDS-1C" << endl
            << W << " " << H << endl
            << MaxGray << endl;

        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++) {
                FOUT << ImageData[r][c] << " ";
            }
            FOUT << endl;
        }
        FOUT.close();
        return true;
    }

    bool Create_Negative() {
        if (H < 0)
            return false;
        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++)
                ImageData[r][c] = MaxGray - ImageData[r][c];
        }
        return true;
    }

    bool Mean_Filter(int FilterSize = 3) {
        // create a new image in memory
        // apply median filter and then copy the result
        // back to the same image
        if (H < 0)
            return false;
        vector<vector<int>> Temp;
        Create(Temp, H, W);
        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++) {
                int k = 0;
                int SUM = 0;
                for (int i = r - FilterSize / 2; i < r + FilterSize / 2; i++) {
                    for (int j = c - FilterSize / 2; j < c + FilterSize / 2; j++) {
                        if (i >= 0 && i < H && j >= 0 && j < W) {
                            SUM += ImageData[i][j];
                            k++;
                        }
                    }
                }
                Temp[r][c] = SUM / k;
            }
        }
        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++) {
                ImageData[r][c] = Temp[r][c];
            }
        }
    }

    bool Median_Filter(int FilterSize = 3) {
        // create a new image in memory
        // apply median filter and then copy the result
        // back to the same image
        if (H < 0)
            return false;
        vector<vector<int>> Temp;
        Create(Temp, H, W);
        int D[100];
        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++) {
                int k = 0;
                for (int i = r - FilterSize / 2; i < r + FilterSize / 2; i++) {
                    for (int j = c - FilterSize / 2; j < c + FilterSize / 2; j++) {
                        if (i >= 0 && i < H && j >= 0 && j < W) {
                            D[k] = ImageData[i][j];
                            k++;
                        }
                    }
                }
                B_Sort(D, k);
                Temp[r][c] = D[k / 2];
            }
        }
        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++) {
                ImageData[r][c] = Temp[r][c];
            }
        }
        return true;
    }

    bool Rotate(double angle) {

        vector<vector<int>> t;
        Create(t, H, W);
        for (int r = 0; r < H; r++)
        {
            for (int c = 0; c < W; c++)
            {
                int row = r * cos(angle) - c * sin(angle);
                int col = r * sin(angle) + c * cos(angle);
                if (row > 0 && row < H && col>0 && col < W)
                    t[row][col] = ImageData[r][c];
            }
        }
        for (int r = 0; r < H; r++)
        {
            for (int c = 0; c < W; c++)
            {
                ImageData[r][c] = t[r][c];
            }
        }

        return true;
    }

    bool RotateClockwise() {



        for (int i = 0; i < H; i++)
            for (int j = i + 1; j < W; j++)
                swap(ImageData[i][j], ImageData[j][i]);

        for (int i = 0; i < H; i++) {
            int low = 0, high = W - 1;
            while (low < high) {
                swap(ImageData[i][low], ImageData[i][high]);
                low++;
                high--;
            }
        }



        return true;
    }

    bool RotateCounterClockwise() {


        for (int i = 0; i < H; i++)
            for (int j = i + 1; j < W; j++)
                swap(ImageData[i][j], ImageData[j][i]);

        for (int i = 0; i < W; i++) {
            int low = 0, high = H - 1;
            while (low < high) {
                swap(ImageData[low][i], ImageData[high][i]);
                low++;
                high--;
            }
        }



        return true;
    }

    bool FlipHorizontal() {

        if (H < 0)
            return false;

        for (int i = 0; i < H; i++) {


            int start = 0;
            int end = W - 1;

            while (start < end) {

                swap(&ImageData[i][start],
                    &ImageData[i][end]);

                start++;
                end--;
            }
        }


        return true;
    }

    bool FlipVertical() {


        for (int i = 0; i < W; i++) {

            int start = 0;
            int end = H - 1;


            while (start < end) {

                swap(&ImageData[start][i], &ImageData[end][i]);


                start++;
                end--;
            }
        }
        return true;
    }

    bool Resize(int NewH, int NewW) {


        H = NewH;
        W = NewW;
        ImageData.resize(NewH, vector<int>(NewW));

        return true;
    }

    bool CropImage(int Left, int Top, int Right, int Bottom) {

        int newH, newW;
        newH = H - Top + Bottom;
        newW = W - Right + Left;


        if (H < 0)
            return false;
        vector<vector<int>> Temp;
        Create(Temp, newH, newW);

        for (int r = Top; r < H - Bottom; r++)
        {
            for (int c = Left; c < W - Right; c++)
            {
                Temp[r][c] = ImageData[r][c];
            }
        }

        Create(ImageData, newH, newW);

        for (int r = 0; r < newH; r++)
        {
            for (int c = 0; c < newW; c++)
            {
                ImageData[r][c] = Temp[r][c];
            }
        }

        H = newH;
        W = newW;



        return true;
    }

    bool ApplyTransformation(double TrasformationMatrix[][3]) {

        return true;
    }
};

int Menu() {
    int choice;
    do {
        cout << endl << "1. Load Image" << endl
            << "2. Save Image" << endl
            << "__________________________" << endl
            << "3. Create Negative Image" << endl
            << "__________________________" << endl
            << "4. Mean Filter" << endl
            << "5. Median Filter" << endl
            << "__________________________" << endl
            << "6. Flip Horizontal" << endl
            << "7. Flip Vertical" << endl
            << "8. Rotate Clockwise by 90 Degree" << endl
            << "9. Rotate Counter Clockwise by 90 degree" << endl
            << "10. Rotate By arbitrary angle" << endl
            << "11. Resize Image" << endl
            << "12. Apply general Linear Transformation" << endl
            << "13. Crop Image" << endl
            << "__________________________" << endl
            << "14. Exit" << endl << endl
            << "Enter Your Choice: ";
        cin >> choice;

        return choice;
    } while (choice < 1 || choice > 14);
}

int main()
{
    PGMImage Image;

    char FileName[100];
    int Choice;

    do {
        Choice = Menu();
        if (Choice == 1) {
            cout << "Enter File Name ";
            cin >> FileName;
            if (Image.LoadImage(FileName))
                cout << "Image Loaded Successfully " << endl;
            else
                cout << "Image Not Loaded Successfully " << endl;
        }
        else if (Choice == 2) {
            cout << "Enter File Name ";
            cin >> FileName;
            if (Image.SaveImage(FileName))
                cout << "Image Saved Successfully " << endl;
            else
                cout << "Image Not Saved Successfully " << endl;

        }
        else if (Choice == 3) {
            if (Image.H < 0)
                cout << "No Image Loaded" << endl;
            else
                Image.Create_Negative();
        }
        else if (Choice == 4) {
            Image.Mean_Filter(5);
        }
        else if (Choice == 5) {
            int S;
            cout << "Filter Size ";
            cin >> S;
            Image.Median_Filter(S);
        }
        else if (Choice == 6) {
            Image.FlipHorizontal();

        }
        else if (Choice == 7) {

            Image.FlipVertical();

        }
        else if (Choice == 8) {

            Image.RotateClockwise();
        }
        else if (Choice == 9) {

            Image.RotateCounterClockwise();
        }
        else if (Choice == 10) {
            double a;
            cout << "enter angle";
            cin >> a;
            Image.Rotate(a);
            

        }
        else if (Choice == 11) {
            int w = 0, h = 0;
            cout << "Enter New Widht ";
            cin >> w;
            cout << "Enter New Height ";
            cin >> h;

            Image.Resize(h, w);

        }
        else if (Choice == 12) {

        }
        else if (Choice == 13) {

            // int t=20, b=50, l=20, r=40;  debug values
            int t, r, l, b;

            cout << "enter top crop";
            cin >> t;

            cout << "enter bottom crop";
            cin >> b;

            cout << "enter left crop";
            cin >> l;

            cout << "enter right crop";
            cin >> r;


            Image.CropImage(l, t, r, b);
        }

    } while (Choice != 14);

    return 0;
}
