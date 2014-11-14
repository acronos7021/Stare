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
$object = new phpAndCppTalk;
$object->init();
//----------------------------------
// Being a client. Test data.
//----------------------------------
$data = $HTTP_RAW_POST_DATA;
$object->request($data);
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
        $this->port = 3456;
        $this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Could not create socket\n");
    }

    public function request($data) {
        $this->sendData($data);
    }
    // send data to C++ using the specifications specified in the init method
    protected function sendData($data)
    {
        socket_connect($this->socket, $this->host, $this->port) or die("Could not connect to server\n");
        //$remaining=0;
        //while ($remaining=socket_write($this->socket,$data+$remaining, strlen($data)) > 0);
        //Blake
		$length = strlen($data);
		while (true) {
	        $sent = socket_write($this->socket, $data, $length);
	        if ($sent === false) {
				echo "Couldn't send";
	            break;
	        }
	        //sleep(2);
	        //echo $data;
	        //if ($sent ==0){
				//echo "exit loop";
				//break;
			//}
	        // Check if the entire message has been sented
	        if ($sent < $length) {  
	            // If not sent the entire message.
	            // Get the part of the message that has not yet been sented as message
	            $data = substr($data, $sent);
	            // Get the length of the not sented part
	            $length -= $sent;
	        } else {
	            break;
	        }
            
		}
		socket_shutdown($this->socket, 1);
		//echo "out of loop";
        //blake end
        $cppdata = socket_read($this->socket, 8000000) or die("something is wrong");
        echo $cppdata;
        socket_close($this->socket);
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
