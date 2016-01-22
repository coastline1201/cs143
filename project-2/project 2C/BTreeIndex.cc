/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
    RC rc;
    if ((rc = pf.open(indexname, mode)) < 0)
    {
        fprintf(stderr, "Error: cannot open the index file!\n");
        return rc;
    }
    char buffer[PageFile::PAGE_SIZE];
    if (pf.endPid() == 0)
    {
        treeHeight = 1;
        rootPid = -1;
        *(int *) buffer = treeHeight;
        *(PageId *)(buffer+sizeof(int)) = rootPid;
        if ((rc = pf.write(0, buffer)) < 0)
        {
            fprintf(stderr, "Error: cannot write to the index file!\n");
            return rc;
        }
        return 0;
    }
    if ((rc = pf.read(0, buffer)) < 0)
    {
        fprintf(stderr, "Error: cannot read the index file!\n");
        return rc;
    }
    treeHeight = *(int *) buffer;
    rootPid = *(PageId *)(buffer+sizeof(int));
    return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
    RC rc;
    char buffer[PageFile::PAGE_SIZE] = {};
    *(int *) buffer = treeHeight;
    *(PageId *)(buffer+sizeof(int)) = rootPid;
    if ((rc = pf.write(0, buffer)) < 0)
    {
        fprintf(stderr, "Error: cannot write to the index file!\n");
        return rc;
    }
    return 0;
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
    if (rootPid == -1)
    {
        BTLeafNode node;
        node.write(1, pf);
        rootPid = 1;
        treeHeight = 1;
    }
    int midkey;
    PageId sibpid;
    if (insert_level(key, rid, rootPid, 1, midkey, sibpid) < 0)
    {
        treeHeight++;
        BTNonLeafNode node;
        node.initializeRoot(rootPid, midkey, sibpid);
        rootPid = pf.endPid();
        node.write(pf.endPid(), pf);
    }
    return 0;
}

RC BTreeIndex::insert_level(int key, const RecordId& rid, PageId pid, int level, int &midkey, PageId &sibpid)
{
    if (level == treeHeight)
    {
        BTLeafNode node;
        node.read(pid, pf);
        if (node.getKeyCount()<BTLeafNode::MAXKEY_NUM)
        {
            node.insert(key, rid);
            node.write(pid, pf);
        }
        else
        {
            BTLeafNode sibling;
            node.insertAndSplit(key, rid, sibling, midkey);
            node.setNextNodePtr(pf.endPid());
            node.write(pid, pf);
            sibpid = pf.endPid();
            sibling.write(pf.endPid(), pf);
            return RC_NODE_FULL;
        }
    }
    else
    {
        BTNonLeafNode node;
        node.read(pid, pf);
        PageId child;
        int eid;
        node.locateChildPtr(key, child, eid);
        int overflowkey;
        PageId sibpid;
        if (insert_level(key, rid, child, level+1, overflowkey, sibpid)<0)
        {
            if (node.getKeyCount()<BTNonLeafNode::MAXKEY_NUM)
            {
                node.insert(overflowkey, sibpid);
                node.write(pid, pf);
            }
            else
            {
                BTNonLeafNode sibling;
                node.insertAndSplit(overflowkey, sibpid, sibling, midkey);
                node.write(pid, pf);
                sibpid = pf.endPid();
                sibling.write(pf.endPid(), pf);
                return RC_NODE_FULL;
            }
        }
    }
    return 0;
}
/*
 * Find the leaf-node index entry whose key value is larger than or 
 * equal to searchKey, and output the location of the entry in IndexCursor.
 * IndexCursor is a "pointer" to a B+tree leaf-node entry consisting of
 * the PageId of the node and the SlotID of the index entry.
 * Note that, for range queries, we need to scan the B+tree leaf nodes.
 * For example, if the query is "key > 1000", we should scan the leaf
 * nodes starting with the key value 1000. For this reason,
 * it is better to return the location of the leaf node entry 
 * for a given searchKey, instead of returning the RecordId
 * associated with the searchKey directly.
 * Once the location of the index entry is identified and returned 
 * from this function, you should call readForward() to retrieve the
 * actual (key, rid) pair from the index.
 * @param key[IN] the key to find.
 * @param cursor[OUT] the cursor pointing to the first index entry
 *                    with the key value.
 * @return error code. 0 if no error.
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
    PageId current = rootPid;
    if (rootPid<0) return -1;
    int eid;
    for (int i = 1; i<treeHeight; i++)
    {
        BTNonLeafNode node;
        node.read(current, pf);
        node.locateChildPtr(searchKey, current, eid);
    }
    BTLeafNode node;
    node.read(current, pf);
    if ((node.locate(searchKey, eid)) < 0)
        return RC_NO_SUCH_RECORD;
    cursor.pid = current;
    cursor.eid = eid;
    return 0;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
    RC rc;
    BTLeafNode node;
    rc = node.read(cursor.pid, pf);
    if (rc<0) return rc;
    node.readEntry(cursor.eid, key, rid);
    if (cursor.eid == node.getKeyCount()-1)
        cursor.eid = 0, cursor.pid = node.getNextNodePtr();
    else cursor.eid++;
        
    return 0;
}
