package railway.test;

import railway.*;

import java.util.HashSet;
import java.util.Set;

import org.junit.Assert;
import org.junit.Test;

/**
 * Basic tests for the {@link Section} implementation class.
 * 
 * Write your own junit4 tests for the class here.
 */
public class SectionTest {
	
	/** Test construction of a typical section that is not at a junction */
    @Test
    public void testTypicalSection() {
        // parameters used to construct the section under test
        int length = 9;
        JunctionBranch endPoint1 =
                new JunctionBranch(new Junction("j1"), Branch.FACING);
        JunctionBranch endPoint2 =
                new JunctionBranch(new Junction("j2"), Branch.NORMAL);
        
        // section to check under tests
        Section section = new Section(length, endPoint1, endPoint2);
        
        // check the method of getLength()
        Assert.assertEquals(length, section.getLength());
        
        // check the method of getEndPoints()
    	Set<JunctionBranch> set = new HashSet<JunctionBranch>();
    	set.add(endPoint1);
    	set.add(endPoint2);
    	Assert.assertEquals(set, section.getEndPoints());
    	
    	//check the method of otherEndPoint()
    	Assert.assertEquals(endPoint1, section.otherEndPoint(endPoint2));
    	Assert.assertEquals(endPoint2, section.otherEndPoint(endPoint1));
    	
    	//check the method of toString()
    	Assert.assertEquals("9 (j1, FACING) (j2, NORMAL)", section.toString());
    	
    	
    	// check that the invariant has been established    	
    	Assert.assertTrue(section.checkInvariant());
    }

	
    
    
    
    
    @Test(expected = NullPointerException.class)
    public void testSectionNullEndPoint() {
    	
    	 // parameters used to construct the section under test
        int length = 9;
        
        // the section under test
        Section section = new Section(length, null, null);
    	}
    
    
    
    
    @Test(expected = IllegalArgumentException.class)
    public void testSectionIllegalArgumentException() {
    	
    	 // parameters used to construct the section under test
        int length = 9;
        JunctionBranch endPoint1 =
                new JunctionBranch(new Junction("j1"), Branch.FACING);
        JunctionBranch endPoint2 =
                new JunctionBranch(new Junction("j1"), Branch.FACING);
        
        // the section under test
        Section section = new Section(length, endPoint1, endPoint2);
        
    }
    
    
    
    
    @Test(expected = IllegalArgumentException.class)
    public void testSectionIllegalArgumentException2() {
    	
    	 // parameters used to construct the section under test
        int length = -9;
        JunctionBranch endPoint1 =
                new JunctionBranch(new Junction("j1"), Branch.FACING);
        JunctionBranch endPoint2 =
                new JunctionBranch(new Junction("j2"), Branch.NORMAL);
        
        // the section under test
        Section section = new Section(length, endPoint1, endPoint2);
        
    }
    
    
    
    @Test(expected = IllegalArgumentException.class)
    public void testOtherIllegalEndPoint() {
    	
    	 // parameters used to construct the section under test
    	int length = 9;
        JunctionBranch endPoint1 =
                new JunctionBranch(new Junction("j1"), Branch.FACING);
        JunctionBranch endPoint2 =
                new JunctionBranch(new Junction("j2"), Branch.NORMAL);
        
        // parameter not on the section
        JunctionBranch endPoint3 =
                new JunctionBranch(new Junction("j3"), Branch.REVERSE);
        
        // the section to check the otherEndPoint()
        Section section = new Section(length, endPoint1, endPoint2);
        
        // otherEndMethod() under the test
        section.otherEndPoint(endPoint3);
        
        // check that the invariant has been established
        Assert.assertTrue(section.checkInvariant());
    	}
    
    
    /** Basic check of the equals method */
    @Test
    public void testEqualsAndHashCode() {

        // parameters used to construct the locations under test
    	int length1 =9;
    	int length2 =5;
    	
        JunctionBranch endPoint1 =
                new JunctionBranch(new Junction("j1"), Branch.FACING);
        JunctionBranch endPoint2 =
                new JunctionBranch(new Junction("j2"), Branch.NORMAL);
        JunctionBranch endPoint3 =
                new JunctionBranch(new Junction("j1"), Branch.REVERSE);
        JunctionBranch endPoint4 =
                new JunctionBranch(new Junction("j3"), Branch.FACING);
        JunctionBranch anotherEndPoint1 =
                new JunctionBranch(new Junction("j1"), Branch.FACING);
        JunctionBranch anotherEndPoint2 =
                new JunctionBranch(new Junction("j2"), Branch.NORMAL);
        
        // sections to use under cases
        Section section1 = new Section(length1, endPoint1, endPoint2);
        Section section2 = new Section(length2, endPoint3, endPoint4);
        Section anotherSection1 = new Section(length1, anotherEndPoint1, anotherEndPoint2);
        
        // equal case: the Two end-points are equivalent 
        // if their junctions and branches are equivalent
        Assert.assertTrue(section1.equals(anotherSection1));

        // unequal case: basic case
        Assert.assertFalse(section1.equals(section2));
        
        // equal case of HashCode 
        Assert.assertEquals(section1.hashCode(), anotherSection1.hashCode());
        
        // unequal case of HashCode
        Assert.assertNotEquals(section1.hashCode(), section2.hashCode());
        
     // check that the invariant has been established
        Assert.assertTrue(section1.checkInvariant());
        Assert.assertTrue(section2.checkInvariant());
        Assert.assertTrue(anotherSection1.checkInvariant());
    }
    
    
    
}
	
	
	