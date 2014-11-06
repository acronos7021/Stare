<?php
/**
 * Created by minhHiep Le.
 * User: MainDesktop
 * Date: 10/18/2014
 * Time: 2:08 PM
 */
//----------------------------------------------------------
//Must create object and call init() to do anything.
// getData() makes php act as a server,
// wrap it in a while loop if you want it to run forever.
// request() makes php act as a client. Auto send.
// Remember to run close method when necessary.
//-----------------------------------------------------------
//$object = new phpAndCppTalk;
//$object->init();
//----------------------------------
// Being a client. Test data.
//----------------------------------
$data = $HTTP_RAW_POST_DATA;
echo($data);
//$object->request($data);
//----------------------------------
// Being a server. Test data.
//----------------------------------
//$result = $object->getData();
//echo $result;
//----------------------------------
//$object->closeSocket();

class phpAndCppTalk
{

    protected $socket;
    protected $host;
    protected $port;
    protected $data;
    //setup host, port, and socket
    public function init()
    {
        $this->host = "127.0.0.1";
        $this->port = 444;
        $this->socket = socket_create(AF_INET, SOCK_STREAM, 0) or die("Could not create socket\n");
    }

    public function request($data) {
        $this->sendData($data);
    }
    // send data to C++ using the specifications specified in the init method
    protected function sendData($data)
    {
        socket_connect($this->socket, $this->host, $this->port) or die("Could not connect to server\n");
        socket_write($this->socket,$data, strlen($data)) or die("couldn't write");
        $this->data = socket_read($this->socket, 3000);
        echo $this->data;

      //  echo "A connection is found!\n";
    }


//------------------------------------------------------------------------------
// being a server. Wrap this around a while loop if you want it to run forever
//-----------------------------------------------------------------------------
      function getData() {
         $this->recvData();
         return $this->data;
     }

    protected function recvData()
    {

        socket_bind($this->socket, $this->host, $this->port) or die("Couldn't bind socket");
        // now actually listening for connection
        // echo "listening, only 3 connections at a time now please\n";
        socket_listen($this->socket, 3) or die("could not setup socket listen");
        $childSocket = socket_accept($this->socket);
        echo "a connection is found\r\n";
        $this->data = socket_read($childSocket, 3000);
        socket_close($childSocket);
    }

    public function closeSocket() {
        socket_close($this->socket);
    }
}

?>
