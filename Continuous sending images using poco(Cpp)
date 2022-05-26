#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <iostream>
#include <string>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>

#include <Windows.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "base64.cpp"



float RandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}




int i = 0;
std::string m_files_path = "C:/path/to/imagefolder/*.png";

std::string do_write(Poco::JSON::Object& obj) {


    
    std::vector<std::string> imageFiles;
    cv::glob(m_files_path, imageFiles);


    cv::Mat image = cv::imread(imageFiles[i]);
    i++;
    if (i == imageFiles.size())
        i = 0;

    /*cv::imshow("image", image);
    cv::waitKey(1);*/

    std::vector<unsigned char> buff;

    cv::imencode(".png", image, buff);

   
    auto base64_png = reinterpret_cast<const unsigned char*>(buff.data());

    std::string body1 = base64_encode(base64_png, buff.size());

    obj.set("photo1", body1);

    std::stringstream ss;
    obj.stringify(ss);

    std::string body;
    body = ss.str();
    return body;
}

int main()
{
    

    const char host[10] = "127.0.0.1";

    const int port = 4001;

    Poco::Net::HTTPClientSession session(host, port);

    // prepare path
    std::string path;

    path = "/images/";

    Poco::JSON::Object obj;


    // send request
    Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, path);

    

    
    try{

        do {

            std::string body = do_write(obj);

            req.setContentType("application/json");
            req.setContentLength(body.length());
            //req.setKeepAlive(true);
            std::cout << body << std::endl;
            session.sendRequest(req) << body;

            // get response
            Poco::Net::HTTPResponse res;
            std::cout << "Response Status = " << res.getStatus() << std::endl;

            std::cout << "Response Reason = " << res.getReason() << std::endl;
            Sleep(10);
        } while (true);

    }
    catch (const Poco::Exception& exc)
    {
        std::cerr <<"ygibgybyy: "<< exc.displayText() << std::endl;
    }

    return 0;

}

