<?php
    if ($_FILES['uploadedfile']['error'] == UPLOAD_ERR_OK               //checks for errors
        && is_uploaded_file($_FILES['uploadedfile']['tmp_name'])) { //checks that file is uploaded
            $userFile = file_get_contents($_FILES['uploadedfile']['tmp_name']); 
    }
    
    
    
    $output = array('userText' => $userFile, 'b' => 2, 'c' => 3, 'd' => 4, 'e' => 5);
    
    $output = json_encode($output);
    // Use Hiep's socket to send this json to the c++
?>    