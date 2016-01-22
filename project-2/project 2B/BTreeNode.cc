#include "BTreeNode.h"

using namespace std;

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{
    RC rc = pf.read(pid, buffer);
    if (rc != 0)
    {
        fprintf(stderr, "Error: Cannot read pagefile\n");
        return rc;
    }
    return 0; }
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{
    RC rc = pf.write(pid, buffer);
    if (rc != 0)
    {
        fprintf(stderr, "Error: Cannot write pagefile\n");
        return rc;
    }
    return 0; }

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{
    return *((int *)buffer); }

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{
    if (getKeyCount()>=MAXKEY_NUM) return RC_NODE_FULL;
    int eid;
    locate(key, eid);
    PageId pid = getNextNodePtr();
    for (int i = getKeyCount(); i > eid; i--)
    {
        memcpy(buffer+sizeof(int)+ENTRY_SIZE*i,buffer+sizeof(int)+ENTRY_SIZE*(i-1),ENTRY_SIZE);
    }
    *(RecordId *)(buffer+sizeof(int)+ENTRY_SIZE*eid) = rid;
    *(int *)(buffer+sizeof(int)+ENTRY_SIZE*eid+sizeof(RecordId)) = key;
    *(PageId *)(buffer+sizeof(int)+ENTRY_SIZE*(getKeyCount()+1)) = pid;
    (*((int *)buffer))++;
    return 0; }

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{
    if (getKeyCount()!=MAXKEY_NUM) return RC_NODE_FULL;
    
    int eid;
    locate(key, eid);
    PageId pid = getNextNodePtr();
    for (int i = getKeyCount(); i > eid; i--)
    {
        memcpy(buffer+sizeof(int)+ENTRY_SIZE*i,buffer+sizeof(int)+ENTRY_SIZE*(i-1),ENTRY_SIZE);
    }
    *(RecordId *)(buffer+sizeof(int)+ENTRY_SIZE*eid) = rid;
    *(int *)(buffer+sizeof(int)+ENTRY_SIZE*eid+sizeof(RecordId)) = key;
    *(PageId *)(buffer+sizeof(int)+ENTRY_SIZE*(getKeyCount()+1)) = pid;
    
    int countl = MAXKEY_NUM/2;
    int countr = MAXKEY_NUM-countl;
    
    //copy to sibling;
    *(int *)sibling.buffer = countr;
    memcpy(sibling.buffer+sizeof(int), buffer+sizeof(int)+ENTRY_SIZE*countl, countr*ENTRY_SIZE);
    sibling.setNextNodePtr(pid);
    siblingKey = *(int *)(sibling.buffer+sizeof(int)+sizeof(RecordId));
    
    //deal with current buffer
    *(int *)buffer = countl;
    
    return 0; }

/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{
    eid = 0;
    int offset = sizeof(int) + sizeof(RecordId);
    for (eid = 0; eid < getKeyCount(); eid++, offset += ENTRY_SIZE)
    {
       if ((*(int *)(&buffer[offset])) >= searchKey)
           break;
    }
    if (eid >= getKeyCount()) return RC_NO_SUCH_RECORD;
    return 0; }

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{
    int offset = sizeof(int)+eid*ENTRY_SIZE;
    rid = *(RecordId *)(buffer+offset);
    key = *(int *)(buffer+offset+sizeof(RecordId));
    return 0; }

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{
    return *(PageId *)(&buffer[sizeof(int)+getKeyCount()*ENTRY_SIZE]); }

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{
    *(PageId *)(&buffer[sizeof(int)+getKeyCount()*ENTRY_SIZE]) = pid;
    return 0; }

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{
    RC rc = pf.read(pid, buffer);
    if (rc != 0)
    {
        fprintf(stderr, "Error: Cannot read pagefile\n");
        return rc;
    }
    return 0; }
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{
    RC rc = pf.write(pid, buffer);
    if (rc != 0)
    {
        fprintf(stderr, "Error: Cannot write pagefile\n");
        return rc;
    }
    return 0; }

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{
    return *((int *)buffer); }


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{
    if (getKeyCount()>=MAXKEY_NUM) return RC_NODE_FULL;
    int eid;
    PageId pidout;
    locateChildPtr(key, pidout, eid);
    
    for (int i = getKeyCount(); i > eid; i--)
    {
        memcpy(buffer+sizeof(int)+sizeof(PageId)+ENTRY_SIZE*i,buffer+sizeof(int)+sizeof(PageId)+ENTRY_SIZE*(i-1),ENTRY_SIZE);
    }
    *(PageId *)(buffer+sizeof(int)+ENTRY_SIZE*(eid+1)) = pid;
    *(int *)(buffer+sizeof(int)+ENTRY_SIZE*eid+sizeof(PageId)) = key;
    (*((int *)buffer))++;
    return 0; }

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{
    if (getKeyCount()>=MAXKEY_NUM) return RC_NODE_FULL;
    int eid;
    PageId pidout;
    locateChildPtr(key, pidout, eid);
    
    for (int i = getKeyCount(); i > eid; i--)
    {
        memcpy(buffer+sizeof(int)+sizeof(PageId)+ENTRY_SIZE*i,buffer+sizeof(int)+sizeof(PageId)+ENTRY_SIZE*(i-1),ENTRY_SIZE);
    }
    *(PageId *)(buffer+sizeof(int)+ENTRY_SIZE*(eid+1)) = pid;
    *(int *)(buffer+sizeof(int)+ENTRY_SIZE*eid+sizeof(PageId)) = key;
    
    int countl = (MAXKEY_NUM+1)/2;
    int countr = (MAXKEY_NUM+1)-countl-1;
    
    midKey = *(int *)(buffer+sizeof(int)+sizeof(PageId)+ENTRY_SIZE*countl);
    
    //copy to sibling;
    *(int *)sibling.buffer = countr;
    memcpy(sibling.buffer+sizeof(int), buffer+sizeof(int)+ENTRY_SIZE*(countl+1), countr*ENTRY_SIZE+sizeof(PageId));
    
    //deal with current buffer
    *(int *)buffer = countl;
    
    return 0; }

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid, int& eid)
{
    eid = 0;
    int offset = sizeof(int) + sizeof(PageId);
    for (eid = 0; eid < getKeyCount(); eid++, offset += ENTRY_SIZE)
    {
        if (eid >= getKeyCount())
            break;
        if ((*(int *)(&buffer[offset])) > searchKey)
            break;
    }
    pid = *(PageId *)(buffer+offset-sizeof(PageId));
    return 0; }

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{
    *(int *)buffer = 1;
    *(PageId *)(buffer+sizeof(int))=pid1;
    *(int *)(buffer+sizeof(int)+sizeof(PageId))=key;
    *(PageId *)(buffer+ENTRY_SIZE)=pid2;
    return 0; }
