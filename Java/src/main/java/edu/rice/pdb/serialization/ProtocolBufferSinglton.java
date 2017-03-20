//package edu.rice.pdb.serialization;
//
//import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP;
//import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP;
//import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP;
//import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP.PartP;
//import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP.SupplierP;
//
//import edu.rice.pdb.serialization.proto.LongVectorProtos.LongVectorP;
//
//public class ProtocolBufferSinglton {
//
//	private static PartP.Builder m_part;
//	private static SupplierP.Builder m_supplier;
//	private static OrderP.Builder  m_order;
//	private static LineItemP.Builder  m_lineItem;
//	private static CustomerP.Builder  m_customer;
//	private static LongVectorP.Builder m_LongVector;
//	
//	private static ProtocolBufferSinglton instance = null;
//	 
//	// Lazy Initialization (If required then only)
//	public static ProtocolBufferSinglton getInstance() {
//		if (instance == null) {
//			// Thread Safe. Might be costly operation in some case
//			synchronized (ProtocolBufferSinglton.class) {
//				if (instance == null) {
//					instance = new ProtocolBufferSinglton();
//				}
//			}
//		}
//		return instance;
//	}
//	
//	
//    protected ProtocolBufferSinglton() {
//		m_part = PartP.newBuilder();
//		m_supplier = SupplierP.newBuilder();
//		m_order = OrderP.newBuilder();
//		m_lineItem = LineItemP.newBuilder();
//		m_customer = CustomerP.newBuilder();
//
//		m_LongVector = LongVectorP.newBuilder();
//	}
//
//	public PartP.Builder getPart() {
//		return m_part;
//	}
//	
//	public SupplierP.Builder getSupplier() {
//		return m_supplier;
//	}
//	
//	public OrderP.Builder getOrder() {
//		return m_order;
//	}
//	
//	public LineItemP.Builder getLineItem() {
//		return m_lineItem;
//	}
//
//	public CustomerP.Builder getCustomer() {
//		return m_customer;
//	}
//	
//	public LongVectorP.Builder getElement() {
//		return m_LongVector;
//	}
//	
//}
