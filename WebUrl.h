 #ifndef WEB_URL_H_
  #define WEB_URL_H_
  
  #include <regex>
  #include <string>
  using namespace std;
  
  namespace crystal {
      class WebUrl {
     public:
         WebUrl(const string& url) : _url(url) {}
         WebUrl(string&& url) : _url(move(url)) {}
 
         string Request(const string& request) const;
     private:
         string _url;
     };
 }
 
 #endif