#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <boost/json.hpp>
#include <boost/json/src.hpp>

//#include <Windows.h>

//#include <opencv2/calib3d/calib3d.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/opencv.hpp>
//

#include <random>
#include <iostream>
namespace spdlog {
    void info(auto&&...) {}
} // namespace spdlog

static double randomFloat(double min, double max) {
    static std::mt19937 prng;
    return std::uniform_real_distribution<double>(min, max)(prng);
}

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http  = beast::http;          // from <boost/beast/http.hpp>
namespace net   = boost::asio;          // from <boost/asio.hpp>
using tcp       = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
namespace bj    = boost::json;
using boost::asio::steady_timer;

static void fail(beast::error_code ec, std::string_view what) {
    std::cerr << ec.message() << ": " << what << std::endl;
    exit(1);
}

class Client : public std::enable_shared_from_this<Client> {
    tcp::resolver                     resolver_;
    beast::tcp_stream                 stream_;
    beast::flat_buffer                buffer_; // (Must persist between reads)
    http::request<http::string_body>  req_;
    http::response<http::string_body> res_;
    int                               m_outer;

  public:
    // Objects are constructed with a strand to
    // ensure that handlers do not execute concurrently.
    explicit Client(net::io_context& ioc)
        : resolver_(net::make_strand(ioc))
        , stream_(net::make_strand(ioc))

    {}
    // void run(char const*, char const*, char const*, int,float);

    void sendData(char const*, char const*, char const*, int);

  private:
    void on_resolve(beast::error_code, tcp::resolver::results_type);
    void on_connect(beast::error_code, tcp::resolver::results_type::endpoint_type);
    void do_write();
    void on_write(beast::error_code, std::size_t);
    void on_read(beast::error_code, std::size_t);
};

void Client::sendData(char const* host, char const* port, char const* target,
                      int version) {

    req_.version(version);
    spdlog::info("running");
    req_.method(http::verb::post);
    req_.target(target);
    req_.set(http::field::host, host);
    req_.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req_.set(http::field::accept, "*/*");

    req_.set(http::field::content_type, "application/json");

   

    resolver_.async_resolve(
        host, port, beast::bind_front_handler(&Client::on_resolve, shared_from_this()));
}

void Client::on_resolve(beast::error_code ec, tcp::resolver::results_type results) {

    if (ec)
        return fail(ec, "resolve");

    stream_.expires_after(std::chrono::milliseconds(10000));

    // Make the connection on the IP address we get from a lookup
    stream_.async_connect(
        results, beast::bind_front_handler(&Client::on_connect, shared_from_this()));
}

void Client::on_connect(beast::error_code ec,
                        tcp::resolver::results_type::endpoint_type) {
    if (ec)
        return fail(ec, "connect");

    do_write();
}

void Client::do_write() {
    std::cout << " ===== do_write ====== " << std::endl;
    
    
     bj::value data = {{"dia", randomFloat(2.3, 2.5)},
                      {"inner", randomFloat(2.2, 2.4)},
                      {"core", randomFloat(2.0, 2.2)},
                      {"sleep_time_in_s", 0.01}};

    req_.body() = bj::serialize(data);
    req_.prepare_payload();
    // Set a timeout on the operation
    stream_.expires_after(std::chrono::milliseconds(1000));

    // Send the HTTP request to the remote host
    http::async_write(stream_, req_,
                      beast::bind_front_handler(&Client::on_write, shared_from_this()));
}

void Client::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, "write");

    // Receive the HTTP response
    http::async_read(stream_, buffer_, res_,
                     beast::bind_front_handler(&Client::on_read, shared_from_this()));
}

void Client::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, "read");

    std::cout << "Response of client :" << res_ << std::endl;

    do_write();
}

int main() {
    net::io_context ioc;
    std::make_shared<Client>(ioc)->sendData("127.0.0.1", "28", "/metrics", 1);
    
    ioc.run();
}
