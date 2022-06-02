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

std::string imageBase64EncodeConvert(cv::Mat image) {
    std::vector<unsigned char> buffer;
    cv::imencode(".png", image, buffer);
    auto base64_png = reinterpret_cast<const unsigned char*>(buffer.data());
    // Poco::JSON::Object obj;
    // obj.set("photo1", buffer);
    
    // std::stringstream ss;
    // obj.stringify(ss);
    // std::string body;
    // body = ss.str();
    // return body;

    std::string image_encode = base64_encode(base64_png, buffer.size());
    return image_encode;
}

std::string stringifyTheBothImages(std::string image1_string) {

    
    Poco::JSON::Object obj;
    obj.set("photo1", image1_string);
    
    std::stringstream ss;
    obj.stringify(ss);
    std::string body;
    body = ss.str();
    return body;
}



/*
If you needed wxternal libraries make sure to download the external libraries.
*/
int main() {
        

    const char host[10] = "127.0.0.1";

    const int port = 4001;

    Poco::Net::HTTPClientSession m_session(host, port);
    m_session.setKeepAlive(true);

    // prepare path
    std::string path;

    path = "/images/";
    Poco::JSON::Object obj;


    // send request
    Poco::Net::HTTPRequest m_request(Poco::Net::HTTPRequest::HTTP_POST, path);

    cv::Mat myImage;
    cv::VideoCapture cap(0);
    while (cap.isOpened()) {
        cap >> myImage;
        if (!myImage.empty()) {
            

            std::string imencode_img = imageBase64EncodeConvert(myImage);
            std::string data = stringifyTheBothImages(imencode_img);

            m_request.setContentType("application/json");
            m_request.setContentLength(data.length());

            m_session.sendRequest(m_request) << data;
            Poco::Net::HTTPResponse res;
            std::cout << "Response Status = " << res.getStatus() << std::endl;

            std::cout << "Response Reason = " << res.getReason() << std::endl;

        }
    }
}
