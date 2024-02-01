/* -*- C++ -*- */
#pragma once
// a full automated WiFi select network
// plugin for arduino menu library
// requires a dynamic menu (MENU_USERAM)
// IO: Serial
// Based on library SDMenu.h by Rui Azevedo [ruihfazevedo@gmail.com]
// Feb 2024 - Sergio Coscolín [coscolin.sergio@gmail.com]

#include <menu.h>

using namespace Menu;

//minimalist WiFi driver (using arduino WiFi scan)
//we avoid allocating memory here, instead we read all info from WiFi.scanNetworks(true)
template<typename SDC>
class FSO {
public:
  using Type=SDC;
  Type& WiFi;
  //idx_t selIdx=0;//preserve selection context, because we preserve folder ctx too
  //we should use filename instead! idx is useful for delete operations thou...

    int n = WiFi.scanNetworks(false); // Wait until scan complete

  FSO(Type& WiFi):WiFi(WiFi) {}

  virtual ~FSO() {
    WiFi.scanDelete();
  }

/*
  String retrieveSSID(int index) {
    //dir.close();
    // Serial.println("reopen dir, context");
    //dir=sdc.open(folderName.c_str());
    return WiFi.SSID(index).c_str();
  }
*/
  // number of WiFi's founded
  int count() {
    // Serial.print("count:");
    return n;
  }

  //get WiFi SSID name selected by index
  String entry(long idx) {
    idx_t cnt=0;
    for (int i = 0; i < n; ++i) {
      // Print SSID for each network found
      Serial.printf("%2d", i + 1);
      Serial.print(" | ");
      Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
      if(idx==cnt++) {
        Serial.print(" | SELECTED\n");
        String n=String(WiFi.SSID(i).c_str());
        return n;
      }
      Serial.println();
    }
    return "";
  }
};

//////////////////////////////////////////////////////////////////////
// WiFi's available cached menu
template<typename SDC,idx_t maxSz>
class CachedFSO:public FSO<SDC> {
public:
  using Type=SDC;
  long cacheStart=0;
  String cache[maxSz];
  long size=0;//folder size (count of files and folders)
  CachedFSO(Type& WiFi):FSO<SDC>(WiFi) {}
  
  void refresh(long start=0) {
    if (start<0) start=0;
    // Serial.print("Refreshing from:");
    // Serial.println(start);
    cacheStart=start;

//    FSO<SDC>::dir.rewindDirectory();
    int n = FSO<SDC>::WiFi.scanNetworks(false); // Wait until scan complete
    size=0;
    for (int i = 0; i < n; ++i) {
      if (start<=size&&size<start+maxSz)
        cache[size-start]=String(WiFi.SSID(i).c_str());
      size++;
    }
  }
  
  //open a folder
  //bool retrieveSSID(folderName) {
  //  if (!FSO<SDC>::retrieveSSID(folderName)) return false;
  //  refresh();
  //  return true;
  //}

  long count() {return size;}

  long entryIdx(String name) {
    idx_t sz=min(count(),(long)maxSz);
    for(int i=0;i<sz;i++)
      if (name==cache[i]) return i+cacheStart;
    long at=FSO<SDC>::entryIdx(name);
    //put cache around the missing item
    refresh(at-(maxSz>>1));
    return at;
  }

  String entry(long idx) {
    if (0>idx||idx>=size) return "";
    if (cacheStart<=idx&&idx<(cacheStart+maxSz)) return cache[idx-cacheStart];
    refresh(idx-(maxSz>>1));
    return entry(idx);
  }
};

////////////////////////////////////////////////////////////////////////////
#include <WiFi.h>
// On this example we assume the existence of an esc button as we are not drawing
// an exit option (or [..] as would be appropriate for a file system)
// not the mennu presents it self as the menu and as the options
// ands does all drawing navigation.
//TODO: we can specialize input too, for typing filename select
  #define USE_BACKDOTS 1

template<typename FS>
class WiFiMenuT:public menuNode,public FS {
public:
  String folderName="/";//set this to other folder when needed
  String selectedFolder="/";
  String selectedSSID="";

  // using menuNode::menuNode;//do not use default constructors as we wont allocate for data
  WiFiMenuT(typename FS::Type& sd,constText* title,const char* at,Menu::action act=doNothing,Menu::eventMask mask=noEvent)
    :menuNode(title,0,NULL,act,mask,
      wrapStyle,(systemStyles)(_menuData|_canNav))
    ,FS(sd)
    {}

  void begin() {}

  //this requires latest menu version to virtualize data tables
  prompt& operator[](idx_t i) const override {return *(prompt*)this;}//this will serve both as menu and as its own prompt
  result sysHandler(SYS_FUNC_PARAMS) override {
    switch(event) {
      case enterEvent:
        if (nav.root->navFocus!=nav.target) {//on sd card entry
          nav.sel=((WiFiMenuT<FS>*)(&item))->entryIdx(((WiFiMenuT<FS>*)(&item))->selectedSSID)+USE_BACKDOTS;//restore context
        }
    }
    return proceed;
  }

  void doNav(navNode& nav,navCmd cmd) {
    switch(cmd.cmd) {
      case enterCmd: if (nav.sel>=USE_BACKDOTS) {
          
          String selFile=WiFiMenuT<FS>::entry(nav.sel-USE_BACKDOTS);

            //Serial.print("\nFile selected:");
            //select a file and return
            selectedSSID=selFile;
            selectedFolder=folderName;
            nav.root->node().event(enterEvent);
            menuNode::doNav(nav,escCmd);
          return;
        }
      case escCmd:
          menuNode::doNav(nav,escCmd);//then exit
        /*  NO ES NECESARIO PORQUE SOLO HAY UN NIVEL DE MENÚ DINAMICO
        if(folderName=="/")//at root?
          menuNode::doNav(nav,escCmd);//then exit
        else {//previous folder
          idx_t at=folderName.lastIndexOf("/",folderName.length()-2)+1;
          String fn=folderName.substring(at,folderName.length()-1);
          folderName.remove(folderName.lastIndexOf("/",folderName.length()-2)+1);
          WiFiMenuT<FS>::goFolder(folderName);
          dirty=true;//redraw menu
          nav.sel=WiFiMenuT<FS>::entryIdx(fn)+USE_BACKDOTS;
        }
        */
        return;
    }
    menuNode::doNav(nav,cmd);
  }

  //print menu and items as this is a virtual data menu
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t pn) {
    if(root.navFocus!=this) {//show given title or filename if selected
      return selectedSSID==""?
        /*menuNode::printTo(root,sel,out,idx,len,pn)*/
        out.printRaw(shadow->text,len):
        out.printRaw(selectedSSID.c_str(),len);
    } else if(idx==-1) {//when menu open (show folder name)
      ((menuNodeShadow*)shadow)->sz=WiFiMenuT<FS>::count()+USE_BACKDOTS;
      idx_t at=folderName.lastIndexOf("/",folderName.length()-2)+1;
      String fn=folderName.substring(at,folderName.length()-1);
      return out.printRaw(fn.c_str(),len);
      // return out.printRaw(folderName.c_str(),len);
      // return out.printRaw(WiFiMenuT<FS>::dir.name(),len);
    }
    //drawing options
    out.setColor(fgColor, sel);
    idx_t i=out.tops[root.level]+idx;
    if (i<USE_BACKDOTS) len-=out.printRaw("[..]",len);
    else len-=out.printRaw(WiFiMenuT<FS>::entry(out.tops[root.level]+idx-USE_BACKDOTS).c_str(),len);
    return len;
  }
};

class WiFiMenu:public WiFiMenuT<FSO<decltype(WiFi)>> {
public:
  WiFiMenu(constText* title,const char* at,Menu::action act=doNothing,Menu::eventMask mask=noEvent)
    :WiFiMenuT<FSO<decltype(WiFi)>>(WiFi,title,at,act,mask) {}
};

template<idx_t cacheSize>
class CachedWiFiMenu:public WiFiMenuT<CachedFSO<decltype(WiFi),cacheSize>> {
public:
  CachedWiFiMenu(constText* title,const char* at,Menu::action act=doNothing,Menu::eventMask mask=noEvent)
    :WiFiMenuT<CachedFSO<decltype(WiFi),cacheSize>>(WiFi,title,at,act,mask) {}
};