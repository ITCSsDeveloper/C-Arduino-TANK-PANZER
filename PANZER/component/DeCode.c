const char * DeCode(int input)
{
  switch(input)
  {
    case 16736925 : return "UP";    break;
    case 16754775 : return "DOWN";  break;
    case 16720605 : return "LEFT";  break;
    case 16761405 : return "RIGHT"; break;
    case 16712445 : return "OK";    break;
    case 16738455 : return "1";     break;
    case 16750695 : return "2";     break;
    case 16756815 : return "3";     break;
    case 16724175 : return "4";     break;
    case 16718055 : return "5";     break;
    case 16743045 : return "6";     break;
    case 16716015 : return "7";     break;
    case 16726215 : return "8";     break;
    case 16734885 : return "9";     break;
    case 16730805 : return "0";     break;
    case 16728765 : return "*";     break;
    case 16732845 : return "#";     break;
    default       : return "..";    break;
  }
}

// แปรรหัส IR ไปเป็น String 