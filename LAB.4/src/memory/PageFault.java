package memory;

/* It is in this file, specifically the replacePage function that will
   be called by MemoryManagement when there is a page fault.  The 
   users of this program should rewrite PageFault to implement the 
   page replacement algorithm.
*/

  // This PageFault file is an example of the FIFO Page Replacement 
  // Algorithm as described in the Memory Management section.

import java.util.*;

public class PageFault {

  static CycledList<Page> cycledList = new CycledList<>();

  private static boolean SwapPage(Vector mem , int virtPageNum , int replacePageNum , ControlPanel controlPanel, Page page) {
      Page newPage = (Page) mem.get(replacePageNum);
      newPage.physical = page.physical;
      controlPanel.removePhysicalPage(page.id);
      cycledList.changeData(newPage);
      controlPanel.addPhysicalPage(page.physical, replacePageNum);
      page.inMemTime = 0;
      page.lastTouchTime = 0;
      page.R = 0;
      page.M = 0;
      page.physical = -1;
      return true;
  }

  public static void replacePage ( Vector mem , int virtPageNum , int replacePageNum , ControlPanel controlPanel ) {
    int t = 10;
    boolean mapped = false, updated = false;

    if (cycledList.getNumber() < (virtPageNum + 1) / 2) {
      Page newPage = (Page) mem.get(replacePageNum);
      newPage.physical = cycledList.getNumber();
      cycledList.add((Page) mem.get(replacePageNum));
      controlPanel.addPhysicalPage(newPage.physical, replacePageNum);
    }
    else {
      Page page = cycledList.get(), start = null;
      while (!mapped) {
        if (page != start) {
          if (page.R == 1) {
            page.R = 0;
            page.lastTouchTime = 0;
            if (start == null) start = page;
            page = cycledList.getNext();
          } else {

            if (page.lastTouchTime > t) {
              if (page.M == 1) {
                page.M = 0;
                updated = true;
                if (start == null) start = page;
                page = cycledList.getNext();
              } else {
                  mapped = SwapPage(mem, virtPageNum, replacePageNum, controlPanel, page);
              }
            } else {
              if (start == null) start = page;
              page = cycledList.getNext();
            }
          }
        } else {
          if (updated) start = null;
          else {
              mapped = SwapPage(mem, virtPageNum, replacePageNum, controlPanel, page);
          }
        }
      }
    }
  }
}
