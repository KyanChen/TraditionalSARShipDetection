#include "LevirSAR.h"
#include "CoarseDetection.h"
#include "FineDetection.h"
#include "SeaLandSeg.h"

#ifdef GET_DATA

#endif // GET_DATA



void strip(char *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    for (i = 0; i < len; ++i) {
        char c = s[i];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') ++offset;
        else s[i - offset] = c;
    }
    s[len - offset] = '\0';
}

#ifndef GET_DATA

int main(int argc, char **argv)
{
    int i;
    for (i = 0; i < argc; ++i) {
        if (!argv[i]) continue;
        strip(argv[i]);
    }

    if (argc < 2) {
        fprintf(stderr, "usage: %s <function>\n", argv[0]);
        return 0;
    }

    char *filename = argv[1];
    char *outputname = argv[2];

	/*SeaLandSeg SLS(filename);
	SLS.get_mask();*/

    Mat img = imread(filename, 0);
    
	clock_t starttime = clock();
	vector<Rect> proposalTargets;
    CoarseDetection CD;
    CD.run(img, proposalTargets);
	clock_t endtime = clock();
	cout << "coarse:" << (endtime - starttime) / 1000. << " s" << endl;
    
	starttime = clock();
    vector<Rect> finalTargets;
    FineDetection FD;
    FD.run(img, proposalTargets, finalTargets);
	endtime = clock();
	cout << "fine:" << (endtime - starttime) / 1000. << " s" << endl;

    for(int i = 0; i < finalTargets.size(); i++)
    {
        rectangle(img, finalTargets[i], Scalar(255), 1);
    }

    imwrite(outputname, img);

    return 0;
}

#endif

#ifdef GET_DATA
int getProposal(string filename)
{
    Mat img = imread(filename, IMREAD_ANYDEPTH);

    img.convertTo(img, CV_8UC1);
    //imwrite(filename.substr(0, filename.length() - 4) + ".jpg", img);

    clock_t starttime = clock();
    vector<Rect> proposalTargets;
    CoarseDetection CD;
    CD.run(img, proposalTargets);
    clock_t endtime = clock();
    cout << "coarse:" << (endtime - starttime) / 1000. << " s" << endl;

    starttime = clock();
    vector<Rect> finalTargets;
    FineDetection FD;
    FD.run(filename, img, proposalTargets, finalTargets);
    endtime = clock();
    cout << "fine:" << (endtime - starttime) / 1000. << " s" << endl;
    return 0;
}
#endif // GET_DATA
