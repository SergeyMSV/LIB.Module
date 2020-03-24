#include <iostream>

#include <string>
#include <vector>

#include <boost/asio.hpp>

#include <utilsBase.h>

template<int DataSizeRecv = 1024>
class tCommunication
{
    boost::asio::serial_port m_Port;

    unsigned char m_Data[DataSizeRecv];

public:
    tCommunication(boost::asio::io_context& io_context, const std::string& portID, utils::tUInt32 portBR)
        : m_Port(io_context)
    {
        m_Port.open(portID);

        if (m_Port.is_open())
        {
            m_Port.set_option(boost::asio::serial_port_base::baud_rate(portBR));
            m_Port.set_option(boost::asio::serial_port_base::character_size(8));
            m_Port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
            m_Port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
            m_Port.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
        }

        Receive();
    }

private:
    void Receive()
    {
        m_Port.async_read_some(boost::asio::buffer(m_Data, sizeof(m_Data)),
            [this](boost::system::error_code ec, std::size_t bytes_recvd)
            {
                //if (!ec && bytes_recvd > 0)
                //{
                //    do_send(bytes_recvd);
                //}
                //else
                //{

                utils::tVectorUInt8 Data(m_Data, m_Data + bytes_recvd);
                Receive();
                OnReceived(Data);//[TBD] is it here or before Receive()???
           
                //}
            });
    }

protected:
    virtual void OnReceived(utils::tVectorUInt8& data) = 0;

//    void do_receive()
//    {
//        socket_.async_receive_from(
//            boost::asio::buffer(data_, max_length), sender_endpoint_,
//            [this](boost::system::error_code ec, std::size_t bytes_recvd)
//            {
//                if (!ec && bytes_recvd > 0)
//                {
//                    do_send(bytes_recvd);
//                }
//                else
//                {
//                    do_receive();
//                }
//            });
//    }
//
//    void do_send(std::size_t length)
//    {
//        socket_.async_send_to(
//            boost::asio::buffer(data_, length), sender_endpoint_,
//            [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/)
//            {
//                do_receive();
//            });
//    }
//
//private:
//    udp::socket socket_;
//    udp::endpoint sender_endpoint_;
//    enum { max_length = 1024 };
//    char data_[max_length];
};